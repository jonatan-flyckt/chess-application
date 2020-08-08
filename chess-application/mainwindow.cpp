#include "mainwindow.h"
#include "ui_mainwindow.h"

//TODO: properly destroy all pointers in all classes with a refactor
//TODO: use the Colour enum instead of bool or string for all classes
//Use difficulty enum with a map from string to enum and back (Only need to add new difficulty in one place)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow){
    _ui->setupUi(this);
    this->setWindowTitle("Escape Chess Engine");

    _new_game_popup = new NewGamePopup();
    connect(_new_game_popup, &NewGamePopup::startNewGame, this, &MainWindow::restartGame);

    _about_popup = new AboutPopup();
    _links_downloads_popup = new LinksDownladsPopup();
    _bug_report_popup = new BugReportPopup();
    _contact_popup = new ContactPopup();

    _board_grid_layout = nullptr;
    _main_grid_layout = nullptr;
    _info_label = nullptr;
    _board_aspect_ratio_widget = nullptr;
    _piece_widget_currently_dragged = nullptr;

    _in_exploration_mode = false;
    _dragging_move_ready_to_complete = false;
    _user_is_white = true;

    _engine = new ChessEngine();

    QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd");
    _game = new ChessGame(_user_is_white, date.toStdString(), Normal, "");

    _legal_moves_for_current_state.clear();
    for (auto legalMove: _game->getLegalMovesForCurrentState()){
        _legal_moves_for_current_state.append(legalMove);
    }


    _dragging_move_in_progress = false;
    _clicking_move_in_progress = false;

    initiateUIComponents();
    _fen_label->setText("Forsyth-Edwards Notation:\n" + QString::fromStdString(_game->getCurrent_state()->_fen_notation));

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    _check_for_player_move_timer = new QTimer(this);
    connect(_check_for_player_move_timer, &QTimer::timeout, this, &MainWindow::checkIfPlayerMadeMove);
    _check_for_player_move_timer->start(HOW_OFTEN_TO_CHECK_FOR_PLAYER_MOVE);

    _time_to_update_board = false;
    _check_for_update_board_timer = new QTimer(this);
    connect(_check_for_update_board_timer, &QTimer::timeout, this, &MainWindow::updateBoardGraphicsAfterMove);
    _check_for_update_board_timer->start(HOW_OFTEN_TO_CHECK_FOR_BOARD_GRAPHIC_UPDATE);
}

void MainWindow::showNewGamePopup(){
    _new_game_popup->setWindowModality(Qt::ApplicationModal);
    _new_game_popup->setWindowFlag(Qt::FramelessWindowHint);
    _new_game_popup->show();
}


void MainWindow::restartGame(Colour colour, Difficulty difficulty, QString name){
    //_check_for_player_move_timer = new QTimer(this);
    //connect(_check_for_player_move_timer, &QTimer::timeout, this, &MainWindow::checkIfPlayerMadeMove);
    _check_for_player_move_timer->start(HOW_OFTEN_TO_CHECK_FOR_PLAYER_MOVE);

    _info_label->setText("New game started");
    _in_exploration_mode = false;
    _user_is_white = colour == White;

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    _game = new ChessGame(_user_is_white, date.toStdString(), difficulty, name.toStdString());
    _difficulty_label->setText("Difficulty: " + QString::fromStdString(stringFromDifficulty(difficulty)));
    if (colour == White)
        _playing_as_colour_label->setPixmap(_graphics_info._white_king.scaled(65, 65, Qt::KeepAspectRatio));
    else
        _playing_as_colour_label->setPixmap(_graphics_info._black_king.scaled(65, 65, Qt::KeepAspectRatio));

    initiateBoardSquaresUI();
    initiatePiecesGraphically();

    clearAlgebraicNotationView();
    _notation_widgets.clear();
    _legal_moves_for_current_state.clear();
    for (auto legalMove: _game->getLegalMovesForCurrentState()){
        _legal_moves_for_current_state.append(legalMove);
    }
    _fen_label->setText("Forsyth-Edwards Notation:\n" + QString::fromStdString(_game->getCurrent_state()->_fen_notation));
    _explore_last_button->setEnabled(false);
    _state_being_viewed = _game->getCurrent_state();

    if (_game->getUser_colour() != _game->getCurrent_state()->_colour_to_move)
        _player_moved_against_engine = true;
    else
        _player_moved_against_engine = false;
}

void MainWindow::resignGame(){
    QMessageBox msgBox(QMessageBox::Question, "Resign", "Do you want to resign the game?",
                       QMessageBox::Yes|QMessageBox::No, this,
                       Qt::FramelessWindowHint);
    int reply = msgBox.exec();
    if (reply == QMessageBox::No)
        return;

    _game->_is_game_over = true;
    QString playerColour = _user_is_white ? "White" : "Black";
    _game->_game_over_reason = (playerColour + " resigned").toStdString();
    _game->_white_won = !_user_is_white;
    _game->_black_won = _user_is_white;
    endGame();
}

void MainWindow::clearAlgebraicNotationView(){
    while(!_algebraic_notation_vertical_layout->isEmpty()) {
        QLayout *moveLayout = _algebraic_notation_vertical_layout->takeAt(0)->layout();
        while(!moveLayout->isEmpty()) {
            QWidget *w = moveLayout->takeAt(0)->widget();
            delete w;
        }
        delete moveLayout;
    }
}

MainWindow::~MainWindow(){
    clearBoardUI();
    if (_board_grid_layout != nullptr)
        delete _board_grid_layout;
    if (_main_grid_layout != nullptr)
        delete _main_grid_layout;
    if (_info_label != nullptr)
        delete _info_label;
    if (_board_aspect_ratio_widget != nullptr)
        delete _board_aspect_ratio_widget;
    delete _ui;
}

void MainWindow::addNotationWidgetForMove(State *resultingState){

    NotationWidget *widget = new NotationWidget(resultingState);
    widget->setStyleSheet("background-color: rgba(255, 255, 255, 0%); border: 0px");
    widget->setFixedHeight(30);
    widget->setFixedWidth(100);
    connect(widget, &NotationWidget::clickedSendState, this, &MainWindow::notationWidgetClicked);
    _notation_widgets.append(widget);
    if (resultingState->_move_to_state._piece._colour == White){
        QHBoxLayout *moveLayout = new QHBoxLayout();
        moveLayout->addWidget(new QLabel(QString::number((int)(resultingState->_move_to_state._move_number+1)/2) + ". "));
        moveLayout->addStretch(1);
        moveLayout->addWidget(widget);
        moveLayout->addStretch(3);
        _algebraic_notation_horizontal_layouts.append(moveLayout);
        _algebraic_notation_vertical_layout->insertLayout(_algebraic_notation_vertical_layout->count()-1, moveLayout);
    }
    else{
        _algebraic_notation_horizontal_layouts.last()->addWidget(widget);
        _algebraic_notation_horizontal_layouts.last()->setStretch(1, 4);
        _algebraic_notation_horizontal_layouts.last()->addStretch(3);
    }
    _algebraic_notation_scroll_area->verticalScrollBar()->setValue(_algebraic_notation_scroll_area->verticalScrollBar()->maximum());
    //_algebraic_notation_scroll_area->ensureWidgetVisible(widget);
}

void MainWindow::notationWidgetClicked(State *state){
    loadStateGraphically(state);
}

void MainWindow::loadStateGraphically(State *state){
    for (auto widget: _notation_widgets){
        widget->setStyleSheet("background-color: rgba(255, 255, 255, 0%); border: 0px");
        if (widget->state()->_number_of_moves == state->_number_of_moves){
            widget->setStyleSheet("background-color: rgba(255, 255, 255, 0%); border: 1px solid blue");
            _algebraic_notation_scroll_area->ensureWidgetVisible(widget);
        }
    }
    _in_exploration_mode = state->_number_of_moves != _game->getCurrent_state()->_number_of_moves;
    _state_being_viewed = state;
    removeAllSquareHighlights();
    _explore_last_button->setEnabled(_in_exploration_mode);
    _explore_next_button->setEnabled(_in_exploration_mode);
    _explore_previous_button->setEnabled(state->_number_of_moves > 0);
    _explore_first_button->setEnabled(state->_number_of_moves > 0);

    if (state->_number_of_moves > 0)
        highlightPreviousMove(state);
    if (state->_white_king_is_in_check || state->_black_king_is_in_check)
        highlightCheck(state);
    clearAllPiecesFromBoard();
    addPiecesToBoardFromState(state);
    _fen_label->setText("Forsyth-Edwards Notation:\n" + QString::fromStdString(state->_fen_notation));
}

void MainWindow::addPiecesToBoardFromState(State *state){
    for (int i = 0; i < state->_board.size(); i++){
        for (int j = 0; j < state->_board.at(i).size(); j++){
            if (state->_board.at(i).at(j) != nullptr){
                Piece *piece = state->_board.at(i).at(j);
                if (piece->_type == Pawn)
                    addPieceGraphically(piece->_colour == White ? _graphics_info._white_pawn : _graphics_info._black_pawn,
                                        QString::fromStdString(squareIDFromIndices(i, j)),
                                        piece->_colour == White ? "white" : "black");
                else if (piece->_type == Rook)
                    addPieceGraphically(piece->_colour == White ? _graphics_info._white_rook : _graphics_info._black_rook,
                                        QString::fromStdString(squareIDFromIndices(i, j)),
                                        piece->_colour == White ? "white" : "black");
                else if (piece->_type == Bishop)
                    addPieceGraphically(piece->_colour == White ? _graphics_info._white_bishop : _graphics_info._black_bishop,
                                        QString::fromStdString(squareIDFromIndices(i, j)),
                                        piece->_colour == White ? "white" : "black");
                else if (piece->_type == Knight)
                    addPieceGraphically(piece->_colour == White ? _graphics_info._white_knight : _graphics_info._black_knight,
                                        QString::fromStdString(squareIDFromIndices(i, j)),
                                        piece->_colour == White ? "white" : "black");
                else if (piece->_type == Queen)
                    addPieceGraphically(piece->_colour == White ? _graphics_info._white_queen : _graphics_info._black_queen,
                                        QString::fromStdString(squareIDFromIndices(i, j)),
                                        piece->_colour == White ? "white" : "black");
                else if (piece->_type == King)
                    addPieceGraphically(piece->_colour == White ? _graphics_info._white_king : _graphics_info._black_king,
                                        QString::fromStdString(squareIDFromIndices(i, j)),
                                        piece->_colour == White ? "white" : "black");
            }
        }
    }
}

void MainWindow::clearAllPiecesFromBoard(){
    for (auto square: _square_widgets){
        while(!square->inner_layout()->isEmpty()) {
            QWidget *w = square->inner_layout()->takeAt(0)->widget();
            delete w;
        }
    }
    _piece_widgets.clear();
}

void MainWindow::resizeEvent(QResizeEvent* event){
   QMainWindow::resizeEvent(event);
   updateFontSizes();
}

void MainWindow::initiatePiecesGraphically(){
    for (int i = 0; i < 8; i++){
        addPieceGraphically(_graphics_info._black_pawn, QString::fromStdString(_cols_from_index.at(i)) + "7", "black");
        addPieceGraphically(_graphics_info._white_pawn, QString::fromStdString(_cols_from_index.at(i)) + "2", "white");
    }

    addPieceGraphically(_graphics_info._white_rook, "a1", "white");
    addPieceGraphically(_graphics_info._white_knight, "b1", "white");
    addPieceGraphically(_graphics_info._white_bishop, "c1", "white");
    addPieceGraphically(_graphics_info._white_queen, "d1", "white");
    addPieceGraphically(_graphics_info._white_king, "e1", "white");
    addPieceGraphically(_graphics_info._white_bishop, "f1", "white");
    addPieceGraphically(_graphics_info._white_knight, "g1", "white");
    addPieceGraphically(_graphics_info._white_rook, "h1", "white");

    addPieceGraphically(_graphics_info._black_rook, "a8", "black");
    addPieceGraphically(_graphics_info._black_knight, "b8", "black");
    addPieceGraphically(_graphics_info._black_bishop, "c8", "black");
    addPieceGraphically(_graphics_info._black_queen, "d8", "black");
    addPieceGraphically(_graphics_info._black_king, "e8", "black");
    addPieceGraphically(_graphics_info._black_bishop, "f8", "black");
    addPieceGraphically(_graphics_info._black_knight, "g8", "black");
    addPieceGraphically(_graphics_info._black_rook, "h8", "black");
}

void MainWindow::highlightLegalSquares(QString originSquare){
    _legal_destination_squares_for_origin_square.clear();
    for (auto move: _legal_moves_for_current_state){
        if (QString::fromStdString(move._origin_square) == originSquare)
            _legal_destination_squares_for_origin_square.append(QString::fromStdString(move._destination_square));
    }
    for (auto squareStr: _legal_destination_squares_for_origin_square){
        for (auto square: _square_widgets){
            if (square->id() == squareStr){
                if (square->getDenotation()  == "white")
                    square->changePixmap(_graphics_info._legal_move_highlight_white);
                else
                    square->changePixmap(_graphics_info._legal_move_highlight_black);
            }
        }
    }
}

void MainWindow::highlightPreviousMove(State *state){
    QVector<QString> previousMoveSquares;
    if (state->_number_of_moves > 0){
        previousMoveSquares.append(QString::fromStdString(state->_move_to_state._origin_square));
        previousMoveSquares.append(QString::fromStdString(state->_move_to_state._destination_square));
    }
    for (auto squareStr: previousMoveSquares){
        for (auto square: _square_widgets){
            if (square->id() == squareStr){
                if (square->getDenotation()  == "white")
                    square->changePixmap(_graphics_info._misc_highlight_white);
                else
                    square->changePixmap(_graphics_info._misc_highlight_black);
            }
        }
    }
}

void MainWindow::removeAllSquareHighlights(){
    for (auto square: _square_widgets){
        if (square->getDenotation()  == "white")
            square->changePixmap(_graphics_info._white_square);
        else
            square->changePixmap(_graphics_info._black_square);
    }
}

void MainWindow::removeHighlightPreviousMove(){
    State *state = _game->getCurrent_state();
    QVector<QString> previousMoveSquares;
    if (state->_number_of_moves > 0){
        previousMoveSquares.append(QString::fromStdString(state->_move_to_state._origin_square));
        previousMoveSquares.append(QString::fromStdString(state->_move_to_state._destination_square));
    }
    for (auto squareStr: previousMoveSquares){
        for (auto square: _square_widgets){
            if (square->id() == squareStr){
                if (square->getDenotation()  == "white")
                    square->changePixmap(_graphics_info._white_square);
                else
                    square->changePixmap(_graphics_info._black_square);
            }
        }
    }
}

void MainWindow::highlightCurrentMovingFromSquare(QString highlightSquare){
    for (auto square: _square_widgets){
        if (square->id() == highlightSquare){
            if (square->getDenotation()  == "white")
                square->changePixmap(_graphics_info._misc_highlight_white);
            else
                square->changePixmap(_graphics_info._misc_highlight_black);
        }
    }
}

void MainWindow::removeHighlightCurrentMovingFromSquare(QString highlightSquare){
    for (auto square: _square_widgets){
        if (square->id() == highlightSquare){
            if (square->getDenotation()  == "white")
                square->changePixmap(_graphics_info._white_square);
            else
                square->changePixmap(_graphics_info._black_square);
        }
    }
}

void MainWindow::removeLegalSquaresHighlight(){
    for (auto squareStr: _legal_destination_squares_for_origin_square){
        for (auto square: _square_widgets){
            if (square->id() == squareStr){
                if (square->getDenotation()  == "white")
                    square->changePixmap(_graphics_info._white_square);
                else
                    square->changePixmap(_graphics_info._black_square);
            }
        }
    }
}

void MainWindow::addPieceGraphically(QPixmap pieceGraphic, QString squareID, QString denotation){
    SquareWidget *squareToPlacePieceOn;
    for (SquareWidget *square: _square_widgets){
        if (square->id() == squareID)
            squareToPlacePieceOn = square;
    }
    PieceWidget *piece = new PieceWidget(denotation);
    piece->setPiece_pixmap(pieceGraphic);
    piece->populateWithPixmap();
    piece->setPiece_position(squareID);
    squareToPlacePieceOn->inner_layout()->addWidget(piece);
    _piece_widgets.append(piece);
}

void MainWindow::removePieceGraphically(PieceWidget *piece){
    int index = _piece_widgets.indexOf(piece);
    _piece_widgets.remove(index);
    piece->hide();
    piece->deleteLater();
}

void MainWindow::initiateUIComponents(){
    _main_grid_layout = new QGridLayout();
    _ui->centralwidget->setLayout(_main_grid_layout);
    _main_grid_layout->setSpacing(0);

    _board_grid_layout = new QGridLayout();
    _board_grid_layout->setSpacing(0);

    _board_aspect_ratio_widget = new BoardAspectRatioWidget(_board_grid_layout, SMALLEST_BOARD_SIZE, SMALLEST_BOARD_SIZE, _ui->centralwidget);
    connect(_board_aspect_ratio_widget, &BoardAspectRatioWidget::getSquareWidgets, this, &MainWindow::sendSquareWidgets);

    _main_grid_layout->setColumnMinimumWidth(BOARD_GRID_COL, SMALLEST_BOARD_SIZE);
    _main_grid_layout->setRowMinimumHeight(BOARD_GRID_ROW, SMALLEST_BOARD_SIZE);
    _main_grid_layout->setColumnStretch(BOARD_GRID_COL, 100);
    _main_grid_layout->setRowStretch(BOARD_GRID_ROW, 100);
    _main_grid_layout->addWidget(_board_aspect_ratio_widget, BOARD_GRID_ROW, BOARD_GRID_COL);
    _board_aspect_ratio_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setLeftLayout();
    setRightLayout();
    setTopLayout();
    initiateBoardSquaresUI();
    initiatePiecesGraphically();
}

void MainWindow::setTopLayout(){
    _top_horizontal_layout = new QHBoxLayout();
    _main_grid_layout->addLayout(_top_horizontal_layout, TOP_LAYOUT_ROW, BOARD_GRID_COL);
    _main_grid_layout->setRowStretch(TOP_LAYOUT_ROW, 0);

    _top_horizontal_layout->addStretch(1);

    _about_button = new QPushButton(this);
    _about_button->setText("About");
    _about_button->setFixedSize(100, 60);
    connect(_about_button, SIGNAL(clicked()), this, SLOT(aboutPopup()));
    _top_horizontal_layout->addWidget(_about_button);

    _links_and_download_button = new QPushButton(this);
    _links_and_download_button->setText("Links / Downloads");
    _links_and_download_button->setFixedSize(100, 60);
    connect(_links_and_download_button, SIGNAL(clicked()), this, SLOT(linksDownloadsPopup()));
    _top_horizontal_layout->addWidget(_links_and_download_button);

    _bug_report_button = new QPushButton(this);
    _bug_report_button->setText("Report a bug");
    _bug_report_button->setFixedSize(100, 60);
    connect(_bug_report_button, SIGNAL(clicked()), this, SLOT(bugReportPopup()));
    _top_horizontal_layout->addWidget(_bug_report_button);

    _contact_button = new QPushButton(this);
    _contact_button->setText("Contact");
    _contact_button->setFixedSize(100, 60);
    connect(_contact_button, SIGNAL(clicked()), this, SLOT(contactPopup()));
    _top_horizontal_layout->addWidget(_contact_button);
    _top_horizontal_layout->addStretch(1);

    _top_horizontal_layout->addStretch(1);
}

void MainWindow::setLeftLayout(){
    _left_vertical_layout = new QVBoxLayout();
    _main_grid_layout->addLayout(_left_vertical_layout, BOARD_GRID_ROW, LEFT_LAYOUT_COL);
    _main_grid_layout->setColumnStretch(LEFT_LAYOUT_COL, 0);

    _left_vertical_layout->addStretch(1);

    _new_game_button = new QPushButton();
    _new_game_button->setText("Start New Game");
    _new_game_button->setFixedSize(160, 40);
    connect(_new_game_button, SIGNAL(clicked()), this, SLOT(showNewGamePopup()));
    _left_vertical_layout->addWidget(_new_game_button);

    _playing_as_horizontal_layout = new QHBoxLayout();
    _playing_as_horizontal_layout->addWidget(new QLabel("Playing as: "));
    _playing_as_colour_label = new QLabel();
    _playing_as_colour_label->setPixmap(_graphics_info._white_king.scaled(65, 65, Qt::KeepAspectRatio));
    _playing_as_horizontal_layout->addWidget(_playing_as_colour_label);
    _left_vertical_layout->addLayout(_playing_as_horizontal_layout);

    _difficulty_label = new QLabel();
    _difficulty_label->setText("Difficulty: " + QString::fromStdString(stringFromDifficulty(_game->getDifficulty())));
    _left_vertical_layout->addWidget(_difficulty_label);

    _resign_game_button = new QPushButton();
    _resign_game_button->setText("Resign Game");
    _resign_game_button->setFixedSize(160, 40);
    connect(_resign_game_button, SIGNAL(clicked()), this, SLOT(resignGame()));
    _left_vertical_layout->addWidget(_resign_game_button);

    _left_vertical_layout->addStretch(1);

    _left_vertical_layout->addWidget(new QLabel("Select Theme:"));
    _theme_combo_box = new QComboBox();
    _theme_combo_box->addItem("Standard");
    _theme_combo_box->setItemText(0, "Standard");
    _theme_combo_box->setFixedWidth(160);
    _left_vertical_layout->addWidget(_theme_combo_box);

    _new_game_button->setEnabled(true);
    _resign_game_button->setHidden(true);
}

void MainWindow::setRightLayout(){
    _algebraic_notation_horizontal_layouts.clear();

    _right_vertical_layout = new QVBoxLayout();
    _main_grid_layout->addLayout(_right_vertical_layout, BOARD_GRID_ROW, RIGHT_LAYOUT_COL);
    _main_grid_layout->setColumnStretch(RIGHT_LAYOUT_COL, 0);

    _export_pgn_notation_button = new QPushButton();
    _export_pgn_notation_button->setText("Export Game Notation (PGN)");
    _export_pgn_notation_button->setFixedHeight(50);
    connect(_export_pgn_notation_button, SIGNAL(clicked()), this, SLOT(exportPGNFile()));
    _right_vertical_layout->addWidget(_export_pgn_notation_button);

    _algebraic_notation_scroll_area = new QScrollArea();
    _algebraic_notation_scroll_area->setFixedWidth(400);
    _algebraic_notation_scroll_area->setMinimumHeight(300);
    _algebraic_notation_scroll_area->setWidgetResizable(true);

    _algebraic_notation_vertical_layout = new QVBoxLayout(_algebraic_notation_scroll_area);
    _algebraic_notation_vertical_layout->addStretch(1);
    _algebraic_notation_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *w = new QWidget();
    w->setLayout(_algebraic_notation_vertical_layout);
    _algebraic_notation_scroll_area->setWidget(w);

    _right_vertical_layout->addWidget(_algebraic_notation_scroll_area);


    _exploration_navigation_horizontal_layout = new QHBoxLayout();
    _explore_first_button = new QPushButton();
    _explore_first_button->setText("<<");
    _explore_first_button->setEnabled(false);
    connect(_explore_first_button, SIGNAL(clicked()), this, SLOT(exploreFirstState()));
    _exploration_navigation_horizontal_layout->addWidget(_explore_first_button);

    _explore_previous_button = new QPushButton();
    _explore_previous_button->setText("<");
    _explore_previous_button->setEnabled(false);
    connect(_explore_previous_button, SIGNAL(clicked()), this, SLOT(explorePreviousState()));
    _exploration_navigation_horizontal_layout->addWidget(_explore_previous_button);

    _explore_next_button = new QPushButton();
    _explore_next_button->setText(">");
    _explore_next_button->setEnabled(false);
    connect(_explore_next_button, SIGNAL(clicked()), this, SLOT(exploreNextState()));
    _exploration_navigation_horizontal_layout->addWidget(_explore_next_button);

    _explore_last_button = new QPushButton();
    _explore_last_button->setText(">>");
    _explore_last_button->setEnabled(false);
    connect(_explore_last_button, SIGNAL(clicked()), this, SLOT(exploreLastState()));
    _exploration_navigation_horizontal_layout->addWidget(_explore_last_button);

    _right_vertical_layout->addLayout(_exploration_navigation_horizontal_layout);

    _colour_to_move_horizontal_layout = new QHBoxLayout();

    _colour_to_move_horizontal_layout->addWidget(new QLabel("Colour to move: "));
    _colour_to_move_label = new QLabel();
    _colour_to_move_label->setPixmap(_graphics_info._white_king.scaled(65, 65, Qt::KeepAspectRatio));
    _colour_to_move_horizontal_layout->addWidget(_colour_to_move_label);
    _colour_to_move_horizontal_layout->addStretch(1);
    _right_vertical_layout->addLayout(_colour_to_move_horizontal_layout);


    _fen_label = new QLabel();
    _fen_label->setFont(_graphics_info._fen_font);
    _fen_label->setTextInteractionFlags(Qt::TextSelectableByMouse);

    _copy_fen = new QPushButton(this);
    _copy_fen->setText("Copy");
    connect(_copy_fen, SIGNAL(clicked()), this, SLOT(copyFENToClipboard()));
    _copy_fen->setFixedSize(40, 20);

    _fen_horizontal_layout = new QHBoxLayout();
    _fen_horizontal_layout->addWidget(_fen_label);
    _fen_horizontal_layout->addWidget(_copy_fen);
    _right_vertical_layout->addLayout(_fen_horizontal_layout);


    _info_label = new QLabel();
    _info_label->setFont(_graphics_info._info_font);
    _info_label->setStyleSheet("color: red");
    _right_vertical_layout->addWidget(_info_label);
}

void MainWindow::exploreFirstState(){
    loadStateGraphically(_game->getState_vector()->at(0));
}

void MainWindow::explorePreviousState(){
    if (_state_being_viewed->_previous_state != nullptr){
        loadStateGraphically(_state_being_viewed->_previous_state);
    }
}

void MainWindow::exploreNextState(){
    if (_state_being_viewed->_next_state != nullptr)
        loadStateGraphically(_state_being_viewed->_next_state);
}

void MainWindow::exploreLastState(){
    if (_game->getState_vector()->size() > 0)
        loadStateGraphically(_game->getState_vector()->at(_game->getState_vector()->size()-1));
}

void MainWindow::exportPGNFile(){
    QString defaultName = QString::fromStdString(_game->getDate()) + "_escape_chess_engine_" + QString::fromStdString(stringFromDifficulty(_game->getDifficulty())).toLower();
    bool saveSuccessful = true;
    try {
        QString defaultFile = QDir::homePath()+"/"+defaultName;
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save file"), defaultFile, tr("Portable Game Notation (*.pgn)"));
        qDebug() << fileName;
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        file.write(QString::fromStdString(_game->getPortable_game_notation()).toUtf8());
        file.close();
    } catch (QException *e) {
        saveSuccessful = false;
    }
    if (saveSuccessful)
        _info_label->setText("File saved");
    else
        _info_label->setText("File save failed\nMake sure that directory exists\nand has write access");
}

void MainWindow::setInfoMessage(QString message){
    _info_label->setText(message);
}

void MainWindow::setCurrentHovered(QString id){
    _currently_hovered_square = id;
}

void MainWindow::startClickingMove(QString originSquare){
    if (_game->_is_game_over || _in_exploration_mode)
        return;
    bool squareContainsPiece = false;
    bool liftingOpponentPiece = false;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == originSquare){
            if ((piece->denotation() == "white" && _game->getUser_colour() == Black) ||
                    (piece->denotation() == "black" && _game->getUser_colour() == White)){
                liftingOpponentPiece = true;
                //TODO: uncomment this when bot exists.
            }
            squareContainsPiece = true;
        }
    }
    if (!squareContainsPiece || liftingOpponentPiece)
        return;
    _clicking_move_in_progress = true;
    _dragging_move_in_progress = false;
    _move_in_progress_origin_square = originSquare;
    highlightLegalSquares(originSquare);
    QString denotation;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == _move_in_progress_origin_square){
            denotation = piece->denotation();
        }
    }
    if ((_game->getCurrent_state()->_colour_to_move == White && denotation == "white") ||
            (_game->getCurrent_state()->_colour_to_move == Black && denotation == "black")){
        highlightCurrentMovingFromSquare(originSquare);
    }
    qDebug() << "started clicking move from: " + originSquare;
}

void MainWindow::completeClickingMove(QString destinationSquare){
    removeLegalSquaresHighlight();
    removeHighlightCurrentMovingFromSquare(_move_in_progress_origin_square);
    if (!_legal_destination_squares_for_origin_square.contains(destinationSquare)){
        qDebug() << "move was not legal";
        _clicking_move_in_progress = false;
        highlightPreviousMove(_game->getCurrent_state());
        if (_game->getCurrent_state()->_white_king_is_in_check || _game->getCurrent_state()->_black_king_is_in_check)
            highlightCheck(_game->getCurrent_state());
        return;
    }
    if (!completeMove(_move_in_progress_origin_square, destinationSquare))
        return;
    _legal_destination_squares_for_origin_square.clear();
    _clicking_move_in_progress = false;
    _move_in_progress_origin_square = "";
    qDebug() << "completed clicking move to: " + destinationSquare;

    _time_to_update_board = true;

}

void MainWindow::getEngineMove(){
    if (((_game->getCurrent_state()->_colour_to_move == Black && _user_is_white) ||
            (_game->getCurrent_state()->_colour_to_move == White && !_user_is_white)) && !_game->_is_game_over){
        //TODO: perform calculations on new thread
        Move move = _engine->selectMoveFromState(_game->getCurrent_state(), _user_is_white ? Black : White);
        performEngineMove(move);
        _time_to_update_board = true;
    }
}

void MainWindow::performEngineMove(Move move){
    completeMove(QString::fromStdString(move._origin_square), QString::fromStdString(move._destination_square));
}

bool MainWindow::completeMove(QString originSquare, QString destinationSquare){
    qDebug() << "in complete move func";

    _info_label->setText("");
    Move moveMade;
    for (auto move: _legal_moves_for_current_state){
        if (QString::fromStdString(move._origin_square) == originSquare &&
                QString::fromStdString(move._destination_square) == destinationSquare) {
            moveMade = move;
            break;
        }
    }
    if (moveMade._move_type == Promotion || moveMade._move_type == PromotionCapture){
        promotedPawnSelection();
        _move_was_promotion = true;
        _promotion_move = moveMade;
    }
    else
        _move_was_promotion = false;
    if (!_game->makeMove(originSquare.toStdString(), destinationSquare.toStdString()))
        return false;

    State *currentState = _game->getCurrent_state();

    _legal_moves_for_current_state.clear();
    for (auto legalMove: _game->getLegalMovesForCurrentState()){
        _legal_moves_for_current_state.append(legalMove);
    }
    _fen_label->setText("Forsyth-Edwards Notation:\n" + QString::fromStdString(_game->getCurrent_state()->_fen_notation));
    if (moveMade._piece._colour == White)
        _colour_to_move_label->setPixmap(_graphics_info._black_king.scaled(65, 65, Qt::KeepAspectRatio));
    else
        _colour_to_move_label->setPixmap(_graphics_info._white_king.scaled(65, 65, Qt::KeepAspectRatio));

    if (_game->_is_game_over){
        endGame();
    }
    else{
        _new_game_button->setEnabled(false);
        _resign_game_button->setHidden(false);
    }
    addNotationWidgetForMove(currentState);

    _explore_first_button->setEnabled(true);
    _explore_previous_button->setEnabled(true);
    _state_being_viewed = currentState;
    for (auto widget: _notation_widgets){
        widget->setStyleSheet("background-color: rgba(255, 255, 255, 0%); border: 0px");
        if (widget->state()->_number_of_moves == currentState->_number_of_moves){
            widget->setStyleSheet("background-color: rgba(255, 255, 255, 0%); border: 1px solid blue");
        }
    }


    if (_game->getUser_colour() != _game->getCurrent_state()->_colour_to_move)
        _player_moved_against_engine = true;
    else
        _player_moved_against_engine = false;


    return true;
}

void MainWindow::endGame(){
    _check_for_player_move_timer->stop();
    QString gameOverString = "";
    gameOverString += "Game is over\n";
    if (_game->_white_won)
        gameOverString += "WHITE WON\n";
    else if (_game->_black_won)
        gameOverString += "BLACK WON\n";
    else
        gameOverString += "DRAW\n";
    gameOverString += QString::fromStdString(_game->_game_over_reason);
    _info_label->setText(gameOverString);
    _new_game_button->setEnabled(true);
    _resign_game_button->setHidden(true);
}

void MainWindow::promotedPawnSelection(){
    Colour colour = _user_is_white ? White : Black;
    _choose_promotion_popup = new ChoosePromotionPopup(this, colour);
    connect(_choose_promotion_popup, &ChoosePromotionPopup::promotionSelected, this, &MainWindow::promotionSelected);
    _choose_promotion_popup->setWindowModality(Qt::ApplicationModal);
    _choose_promotion_popup->setWindowFlag(Qt::FramelessWindowHint);
    _choose_promotion_popup->exec();
    _game->setPiece_selected_from_promotion(_piece_selected_for_promotion);
    delete _choose_promotion_popup;
}

void MainWindow::promotionSelected(PieceType type){
    _piece_selected_for_promotion = type;
}

void MainWindow::aboutPopup(){
    _about_popup->setWindowTitle("About");
    _about_popup->show();
}

void MainWindow::linksDownloadsPopup(){
    _links_downloads_popup->setWindowTitle("Links / Downloads");
    _links_downloads_popup->show();
}

void MainWindow::bugReportPopup(){
    _bug_report_popup->setWindowTitle("Bug report");
    _bug_report_popup->show();
}

void MainWindow::contactPopup(){
    _contact_popup->setWindowTitle("Contact");
    _contact_popup->show();
}

void MainWindow::checkIfPlayerMadeMove(){
    qDebug() << "checking if player made move";
    if (_player_moved_against_engine){
        _player_moved_against_engine = false;
        getEngineMove();
    }
}

void MainWindow::updateBoardGraphicsAfterMove(){
    if (!_time_to_update_board)
        return;
    _time_to_update_board = false;
    loadStateGraphically(_game->getCurrent_state());
}

void MainWindow::highlightCheck(State *state){
    if (!_in_exploration_mode && !_game->_is_game_over)
        _info_label->setText("Check");
    for (auto square: _square_widgets){
        if (square->id() == QString::fromStdString(state->_square_under_check)){
            if (square->getDenotation() == "white")
                square->changePixmap(_graphics_info._check_highlight_white);
            else
                square->changePixmap(_graphics_info._check_highlight_black);
        }
    }
}

void MainWindow::doNotHightlightCheck(){
    for (auto square: _square_widgets){
        if (square->id() == QString::fromStdString(_game->_square_under_check)){
            if (square->getDenotation() == "white")
                square->changePixmap(_graphics_info._white_square);
            else
                square->changePixmap(_graphics_info._black_square);
        }
    }
}

void MainWindow::startDraggingMove(QString originSquare){
    if (_game->_is_game_over || _in_exploration_mode || _game->getCurrent_state()->_colour_to_move != _game->getUser_colour())
        return;
    bool squareContainsPiece = false;
    bool liftingOpponentPiece = false;
    for (auto piece: _piece_widgets){

        if (piece->piece_position() == originSquare){
            if ((piece->denotation() == "white" && _game->getUser_colour() == Black) ||
                    (piece->denotation() == "black" && _game->getUser_colour() == White)){
                liftingOpponentPiece = true;
                //TODO: uncomment this when bot exists.
            }
            squareContainsPiece = true;
        }
    }
    if (!squareContainsPiece || liftingOpponentPiece)
        return;
    _clicking_move_in_progress = false;
    _dragging_move_in_progress = true;
    _move_in_progress_origin_square = originSquare;

    highlightLegalSquares(originSquare);

    QSize widgetSize;
    PieceWidget *pieceToMove;
    QString denotation;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == originSquare){
            widgetSize = piece->size();
            _pixmap_of_dragged_piece = piece->piece_pixmap();
            denotation = piece->denotation();
            pieceToMove = piece;
            break;
        }
    }

    _piece_widget_currently_dragged = new PieceWidget(denotation);
    _piece_widget_currently_dragged->setPiece_pixmap(_pixmap_of_dragged_piece);
    _piece_widget_currently_dragged->setPiece_position("");
    _piece_widget_currently_dragged->populateWithPixmap();
    _piece_widget_currently_dragged->resize(widgetSize.width(), widgetSize.height());
    _piece_widget_currently_dragged->setWindowFlag(Qt::FramelessWindowHint);
    _piece_widget_currently_dragged->setAttribute(Qt::WA_TranslucentBackground);
    int newX = QCursor::pos().x() - widgetSize.width()/2;
    int newY = QCursor::pos().y() - widgetSize.height()/2;
    _piece_widget_currently_dragged->move(newX, newY);
    _piece_widget_currently_dragged->show();
    _piece_widget_currently_dragged->setAttribute(Qt::WA_TransparentForMouseEvents);

    _piece_widgets.append(_piece_widget_currently_dragged);

    SquareWidget *squareFrom;
    for (auto square: _square_widgets){
        if (square->id() == originSquare)
            squareFrom = square;
    }

    QString pieceColour = _piece_widget_currently_dragged->denotation();
    if ((_game->getCurrent_state()->_colour_to_move == White && pieceColour == "black") ||
            (_game->getCurrent_state()->_colour_to_move == Black && pieceColour == "white")){
        QPixmap highlightSquarePixmap = squareFrom->getDenotation() == "white" ? _graphics_info._white_square : _graphics_info._black_square;
        pieceToMove->setPiece_pixmap(highlightSquarePixmap);
        pieceToMove->populateWithPixmap();
    }
    else{
        QPixmap highlightSquarePixmap = squareFrom->getDenotation() == "white" ? _graphics_info._misc_highlight_white : _graphics_info._misc_highlight_black;
        pieceToMove->setPiece_pixmap(highlightSquarePixmap);
        pieceToMove->populateWithPixmap();
    }
    _piece_widget_of_moved_from_square = pieceToMove;

    qDebug() << "started dragging move from: " + originSquare;
}

void MainWindow::setDraggingMoveReadyToComplete(){
    _dragging_move_ready_to_complete = true;
}

void MainWindow::completeDraggingMove(){
    _dragging_move_ready_to_complete = false;
    removeLegalSquaresHighlight();
    removeHighlightCurrentMovingFromSquare(_move_in_progress_origin_square);
    if (!_legal_destination_squares_for_origin_square.contains(_currently_hovered_square)){
        qDebug() << "move was not legal";
        _dragging_move_in_progress = false;
        removePieceGraphically(_piece_widget_currently_dragged);
        _piece_widget_currently_dragged = nullptr;
        _time_to_update_board = true;
        return;
    }
    if (!completeMove(_move_in_progress_origin_square, _currently_hovered_square)){
        qDebug() << "move was not legal";
        _dragging_move_in_progress = false;
        removePieceGraphically(_piece_widget_currently_dragged);
        _piece_widget_currently_dragged = nullptr;
        _time_to_update_board = true;
        return;
    }
    _legal_destination_squares_for_origin_square.clear();
    _dragging_move_in_progress = false;
    removePieceGraphically(_piece_widget_currently_dragged);
    _piece_widget_currently_dragged = nullptr;
    qDebug() << "completed dragging move to: " + _currently_hovered_square;
    _time_to_update_board = true;
}

bool MainWindow::sendClickingMoveStatus(){
    return _clicking_move_in_progress;
}

bool MainWindow::sendDraggingMoveStatus(){
    return _dragging_move_in_progress;
}

bool MainWindow::sendDraggingMoveReadyToCompleteStatus(){
    return _dragging_move_ready_to_complete;
}

QString MainWindow::sendHoveredSquare(){
    return _currently_hovered_square;
}

QString MainWindow::sendMoveOriginSquare(){
    return _move_in_progress_origin_square;
}

void MainWindow::movePieceWidget(QPoint mousePos){
    if (_piece_widget_currently_dragged == nullptr)
        return;
    QSize widgetSize =  _piece_widget_currently_dragged->size();
    int newX = mousePos.x() - widgetSize.width()/2;
    int newY = mousePos.y() - widgetSize.height()/2;
    _piece_widget_currently_dragged->move(newX, newY);
}

void MainWindow::copyFENToClipboard(){
    qDebug() << "clicked copy FEN";
    QClipboard *clipBoard = QApplication::clipboard();
    clipBoard->setText(_fen_label->text().split('\n').at(1));
}

QVector<SquareWidget *> MainWindow::sendSquareWidgets(){
    return _square_widgets;
}

bool MainWindow::mouseIsInsideBoard(){
    QPoint upperLeft;
    QPoint lowerRight;
    SquareWidget *upperLeftSquare;
    SquareWidget *lowerRightSquare;
    for (auto square: _square_widgets){
        if (square->id() == "a8"){
            if (_user_is_white)
                upperLeftSquare = square;
            else
                lowerRightSquare = square;
        }
        if (square->id() == "h1"){
            if (_user_is_white)
                lowerRightSquare = square;
            else
                upperLeftSquare = square;
        }
    }

    upperLeft = upperLeftSquare->parentWidget()->mapToGlobal(upperLeftSquare->pos());
    lowerRight.setX(lowerRightSquare->parentWidget()->mapToGlobal(lowerRightSquare->pos()).x() + lowerRightSquare->size().width());
    lowerRight.setY(lowerRightSquare->parentWidget()->mapToGlobal(lowerRightSquare->pos()).y() + lowerRightSquare->size().height());
    QPoint mousePos = mapTo(this, QCursor::pos());

    return (mousePos.x() > upperLeft.x() && mousePos.y() > upperLeft.y() &&
            mousePos.y() < lowerRight.y() && mousePos.x() < lowerRight.x());
}

void MainWindow::initiateBoardSquaresUI(){
    clearBoardUI();
    for (int i = 0; i < 8; i++){
        if (_user_is_white){
            for (int j = 8; j > 0; j--){
                SquareWidget *square;
                if ((i+j) % 2 == 0){
                    square = new SquareWidget(QString::fromStdString(_cols_from_index.at(j-1)) + QString::number(i+1), _graphics_info._white_square, "white");
                    _board_grid_layout->addWidget(square, abs(8-i), j);
                    _square_widgets.append(square);
                }
                else{
                    square = new SquareWidget(QString::fromStdString(_cols_from_index.at(j-1)) + QString::number(i+1), _graphics_info._black_square, "black");
                    _board_grid_layout->addWidget(square, abs(8-i), j);
                    _square_widgets.append(square);
                }
                connectSquareToSignals(square);
            }
        }
        else{
            for (int j = 0; j < 8; j++){
                SquareWidget *square;
                if ((i+j) % 2 == 0){
                    square = new SquareWidget(QString::fromStdString(_cols_from_index.at(j)) + QString::number(i+1), _graphics_info._black_square, "black");
                    _board_grid_layout->addWidget(square, i+1, abs(8-j));
                    _square_widgets.append(square);
                }
                else{
                    square = new SquareWidget(QString::fromStdString(_cols_from_index.at(j)) + QString::number(i+1), _graphics_info._white_square, "white");
                    _board_grid_layout->addWidget(square, i+1, abs(8-j));
                    _square_widgets.append(square);
                }
                connectSquareToSignals(square);
            }
        }
    }
    addCoordinateWidgets();

}

void MainWindow::connectSquareToSignals(SquareWidget *square){
    connect(square, &SquareWidget::signalCurrentHovered, this, &MainWindow::setCurrentHovered);
    connect(square, &SquareWidget::signalStartClickingMove, this, &MainWindow::startClickingMove);
    connect(square, &SquareWidget::signalCompleteClickingMove, this, &MainWindow::completeClickingMove);
    connect(square, &SquareWidget::signalStartDraggingMove, this, &MainWindow::startDraggingMove);
    connect(square, &SquareWidget::signalCompleteDraggingMove, this, &MainWindow::completeDraggingMove);
    connect(square, &SquareWidget::getClickingMoveStatus, this, &MainWindow::sendClickingMoveStatus);
    connect(square, &SquareWidget::getDraggingMoveStatus, this, &MainWindow::sendDraggingMoveStatus);
    connect(square, &SquareWidget::signalMovePieceWidget, this, &MainWindow::movePieceWidget);
    connect(square, &SquareWidget::signalDraggingMoveReadyToComplete, this, &MainWindow::setDraggingMoveReadyToComplete);
    connect(square, &SquareWidget::getDraggingMoveReadyToCompleteStatus, this, &MainWindow::sendDraggingMoveReadyToCompleteStatus);
    connect(square, &SquareWidget::getMoveOriginSquare, this, &MainWindow::sendMoveOriginSquare);
    connect(square, &SquareWidget::getHoveredSquare, this, &MainWindow::sendHoveredSquare);
    connect(square, &SquareWidget::signalMouseIsInsideBoard, this, &MainWindow::mouseIsInsideBoard);
}

void MainWindow::addCoordinateWidgets(){
    updateFontSizes();
    for (int i = 1; i < 9; i++){
        if (_user_is_white){
            QLabel *number1 = new QLabel(QString::number(abs(9-i)) + " ");
            QLabel *number2 = new QLabel(" " + QString::number(abs(9-i)));
            number1->setFont(_graphics_info._header_font);
            number2->setFont(_graphics_info._header_font);
            _board_grid_layout->addWidget(number1, i, 0, Qt::AlignRight);
            _board_grid_layout->addWidget(number2, i, 9, Qt::AlignLeft);
            QLabel *letter1 = new QLabel(QString::fromStdString(_cols_from_index.at(i-1)));
            QLabel *letter2 = new QLabel(QString::fromStdString(_cols_from_index.at(i-1)));
            letter1->setFont(_graphics_info._header_font);
            letter2->setFont(_graphics_info._header_font);
            _board_grid_layout->addWidget(letter1, 0, i, Qt::AlignHCenter | Qt::AlignBottom);
            _board_grid_layout->addWidget(letter2, 9, i, Qt::AlignHCenter | Qt::AlignTop);
            _board_header_labels.append(number1);
            _board_header_labels.append(number2);
            _board_header_labels.append(letter1);
            _board_header_labels.append(letter2);
        }
        else {
            QLabel *number1 = new QLabel(QString::number(i) + " ");
            QLabel *number2 = new QLabel(" " + QString::number(i));
            number1->setFont(_graphics_info._header_font);
            number2->setFont(_graphics_info._header_font);
            _board_grid_layout->addWidget(number1, i, 0, Qt::AlignRight);
            _board_grid_layout->addWidget(number2, i, 9, Qt::AlignLeft);
            QLabel *letter1 = new QLabel(QString::fromStdString(_cols_from_index.at(abs(8-i))));
            QLabel *letter2 = new QLabel(QString::fromStdString(_cols_from_index.at(abs(8-i))));
            letter1->setFont(_graphics_info._header_font);
            letter2->setFont(_graphics_info._header_font);
            _board_grid_layout->addWidget(letter1, 0, i, Qt::AlignHCenter | Qt::AlignBottom);
            _board_grid_layout->addWidget(letter2, 9, i, Qt::AlignHCenter | Qt::AlignTop);
            _board_header_labels.append(number1);
            _board_header_labels.append(number2);
            _board_header_labels.append(letter1);
            _board_header_labels.append(letter2);
        }
    }
}

void MainWindow::updateFontSizes(){
    int boardSize = std::min(_board_aspect_ratio_widget->size().width(), _board_aspect_ratio_widget->size().height());
    float scaleFactor = float(boardSize) / SMALLEST_BOARD_SIZE;

    if (int(scaleFactor*HEADER_FONT_SIZE) == 0)
        _graphics_info._header_font.setPointSize(START_HEADER_FONT_SIZE);
    else
        _graphics_info._header_font.setPointSize(int(scaleFactor*HEADER_FONT_SIZE));
    for (auto label: _board_header_labels)
        label->setFont(_graphics_info._header_font);

    if (int(scaleFactor*INFO_FONT_SIZE) == 0)
        _graphics_info._info_font.setPointSize(START_INFO_FONT_SIZE);
    else
        _graphics_info._info_font.setPointSize(int(scaleFactor*INFO_FONT_SIZE));
    _info_label->setFont(_graphics_info._info_font);
}

void MainWindow::clearBoardUI(){
    for (auto piece: _piece_widgets)
        delete piece;
    _piece_widgets.clear();
    for (auto label: _board_header_labels)
        delete label;
    _board_header_labels.clear();
    for (auto square: _square_widgets)
        delete square;
    _square_widgets.clear();
}
