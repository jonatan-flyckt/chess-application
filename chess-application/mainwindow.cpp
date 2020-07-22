#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow){
    _ui->setupUi(this);

    _board_grid_layout = nullptr;
    _main_grid_layout = nullptr;
    _set_white_button = nullptr;
    _set_black_button = nullptr;
    _info_label = nullptr;
    _board_aspect_ratio_widget = nullptr;
    _piece_widget_currently_dragged = nullptr;

    _dragging_move_ready_to_complete = false;
    _user_is_white = true;
    _game = new ChessGame(_user_is_white);

    _legal_moves_for_current_state.clear();
    for (auto legalMove: _game->getLegalMovesForCurrentState()){
        _legal_moves_for_current_state.append(legalMove);
    }


    _dragging_move_in_progress = false;
    _clicking_move_in_progress = false;

    initiateUIComponents();
    _fen_label->setText("FEN:\n" + QString::fromStdString(_game->getCurrent_state()->_fen_notation));

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

void MainWindow::restartGame(Colour colour){
    _info_label->setText("New game started");
    _user_is_white = colour == White;
    _game = new ChessGame(_user_is_white);
    _legal_moves_for_current_state.clear();
    for (auto legalMove: _game->getLegalMovesForCurrentState()){
        _legal_moves_for_current_state.append(legalMove);
    }
    _fen_label->setText("FEN:\n" + QString::fromStdString(_game->getCurrent_state()->_fen_notation));
}

MainWindow::~MainWindow(){
    clearBoardUI();
    if (_board_grid_layout != nullptr)
        delete _board_grid_layout;
    if (_main_grid_layout != nullptr)
        delete _main_grid_layout;
    if (_set_white_button != nullptr)
        delete _set_white_button;
    if (_set_black_button != nullptr)
        delete _set_black_button;
    if (_info_label != nullptr)
        delete _info_label;
    if (_board_aspect_ratio_widget != nullptr)
        delete _board_aspect_ratio_widget;
    delete _ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   updateFontSizes();
}

void MainWindow::initiatePiecesGraphically(){
    for (int i = 0; i < 8; i++){
        addPieceGraphically(_graphics_info._black_pawn, colsFromIndex.at(i) + "7", "black");
        addPieceGraphically(_graphics_info._white_pawn, colsFromIndex.at(i) + "2", "white");
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

void MainWindow::highlightPreviousMove(){
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
                    square->changePixmap(_graphics_info._misc_highlight_white);
                else
                    square->changePixmap(_graphics_info._misc_highlight_black);
            }
        }
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

    _about_button = new QPushButton(this);
    _about_button->setText("About");
    _about_button->setFixedSize(80, 40);
    _top_horizontal_layout->addWidget(_about_button);

    _contact_button = new QPushButton(this);
    _contact_button->setText("Contact");
    _contact_button->setFixedSize(80, 40);
    _top_horizontal_layout->addWidget(_contact_button);
    _top_horizontal_layout->addStretch(1);
}

void MainWindow::setLeftLayout(){
    _left_vertical_layout = new QVBoxLayout();
    _left_vertical_layout->setAlignment(Qt::AlignTop);
    _main_grid_layout->addLayout(_left_vertical_layout, BOARD_GRID_ROW, LEFT_LAYOUT_COL);
    _main_grid_layout->setColumnStretch(LEFT_LAYOUT_COL, 0);

    _new_game_button = new QPushButton();
    _new_game_button->setText("Start New Game");
    _new_game_button->setFixedSize(160, 40);
    _left_vertical_layout->addWidget(_new_game_button);


    _set_white_button = new QPushButton(this);
    _set_white_button->setText("Play as white");
    connect(_set_white_button, SIGNAL(clicked()), this, SLOT(setPlayerWhite()));
    _set_white_button->setEnabled(false);
    _set_white_button->setFixedSize(80, 20);

    _set_black_button = new QPushButton(this);
    _set_black_button->setText("Play as black");
    connect(_set_black_button, SIGNAL(clicked()), this, SLOT(setPlayerBlack()));
    _set_black_button->setFixedSize(80, 20);

    _play_as_layout = new QHBoxLayout();
    _play_as_layout->addWidget(_set_white_button);
    _play_as_layout->addWidget(_set_black_button);

    _left_vertical_layout->addLayout(_play_as_layout);

    _left_vertical_layout->addStretch(1);

}

void MainWindow::setRightLayout(){
    _right_vertical_layout = new QVBoxLayout();
    _main_grid_layout->addLayout(_right_vertical_layout, BOARD_GRID_ROW, RIGHT_LAYOUT_COL);
    _main_grid_layout->setColumnStretch(RIGHT_LAYOUT_COL, 0);

    _algebraic_notation_scroll_area = new QScrollArea();
    _algebraic_notation_scroll_area->setFixedWidth(400);
    _algebraic_notation_scroll_area->setMinimumHeight(300);
    _algebraic_notation_scroll_area->setWidgetResizable(true);

    _algebraic_notation_vertical_layout = new QVBoxLayout(_algebraic_notation_scroll_area);
    _algebraic_notation_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //_algebraic_notation_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget *w = new QWidget();
    w->setLayout(_algebraic_notation_vertical_layout);
    _algebraic_notation_scroll_area->setWidget(w);

    _right_vertical_layout->addWidget(_algebraic_notation_scroll_area);


    for (int i = 0; i < 40; i++)
        _algebraic_notation_vertical_layout->addWidget(new QLabel("hej"));
    _algebraic_notation_vertical_layout->addStretch(1);
    //_algebraic_notation_scroll_area->show();


    _info_label = new QLabel();
    _info_label->setFont(_graphics_info._info_font);
    _right_vertical_layout->addWidget(_info_label);

    _fen_label = new QLabel();
    _fen_label->setFont(_graphics_info._fen_font);
    _fen_label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    _right_vertical_layout->addWidget(_fen_label);

    _copy_fen = new QPushButton(this);
    _copy_fen->setText("Copy FEN");
    connect(_copy_fen, SIGNAL(clicked()), this, SLOT(copyFENToClipboard()));
    _copy_fen->setFixedSize(60, 20);
    _right_vertical_layout->addWidget(_copy_fen);
}

void MainWindow::setInfoMessage(QString message){
    _info_label->setText(message);
}

void MainWindow::setCurrentHovered(QString id){
    _currently_hovered_square = id;
}

void MainWindow::startClickingMove(QString originSquare){
    if (_game->_is_game_over)
        return;
    bool squareContainsPiece = false;
    bool liftingOpponentPiece = false;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == originSquare){
            if ((piece->denotation() == "white" && _game->getUser_colour() == Black) ||
                    (piece->denotation() == "black" && _game->getUser_colour() == White)){
                //liftingOpponentPiece = true;
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
        highlightPreviousMove();
        if (_game->getCurrent_state()->_white_king_is_in_check || _game->getCurrent_state()->_black_king_is_in_check)
            highlightCheck();
        return;
    }
    if (!completeMove(destinationSquare))
        return;
    _legal_destination_squares_for_origin_square.clear();
    PieceWidget *pieceToMove;
    QPixmap pieceGraphic;
    QString denotation;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == _move_in_progress_origin_square){
            pieceToMove = piece;
            pieceGraphic = pieceToMove->piece_pixmap();
            denotation = pieceToMove->denotation();
        }
    }
    addPieceGraphically(pieceGraphic, destinationSquare, denotation);
    removePieceGraphically(pieceToMove);
    _clicking_move_in_progress = false;
    _move_in_progress_origin_square = "";
    if (_move_was_promotion)
        performPawnPromotionGraphically(_promotion_move);
    qDebug() << "completed clicking move to: " + destinationSquare;
}

bool MainWindow::completeMove(QString destinationSquare){
    _info_label->setText("");
    Move moveMade;
    removeHighlightPreviousMove();
    for (auto move: _legal_moves_for_current_state){
        if (QString::fromStdString(move._origin_square) == _move_in_progress_origin_square &&
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
    if (!_game->makeMove(_move_in_progress_origin_square.toStdString(), destinationSquare.toStdString()))
        return false;

    //Move the rook if castled (king is moved in normal move function)
    if (moveMade._move_type == LongCastle || moveMade._move_type == ShortCastle)
        moveRookForCastlingGraphically(moveMade);
    if (moveMade._move_type == Capture || moveMade._move_type == PromotionCapture)
        removeCapturedPieceGraphically(moveMade);
    if (moveMade._move_type == EnPassant)
        removeEnPassantCapturedPieceGraphically(moveMade);

    State *currentState = _game->getCurrent_state();
    currentState->_white_king_is_in_check || currentState->_black_king_is_in_check  ? highlightCheck() : doNotHightlightCheck();

    _legal_moves_for_current_state.clear();
    for (auto legalMove: _game->getLegalMovesForCurrentState()){
        _legal_moves_for_current_state.append(legalMove);
    }
    highlightPreviousMove();
    _fen_label->setText("FEN:\n" + QString::fromStdString(_game->getCurrent_state()->_fen_notation));

    if (_game->_is_game_over){
        QString gameOverString = "";
        gameOverString += "Game is over\n";
        if (_game->_white_won)
            gameOverString += "win for WHITE\n";
        else if (_game->_black_won)
            gameOverString += "win for BLACK\n";
        else
            gameOverString += "DRAW\n";
        gameOverString += "by " + QString::fromStdString(_game->_game_over_reason);
        _info_label->setText(gameOverString);
    }
    return true;
}

void MainWindow::promotedPawnSelection(){
    QMessageBox msgBox;
    msgBox.setWindowFlag(Qt::FramelessWindowHint);
    msgBox.setText(tr("Pawn promotion:"));
    msgBox.setInformativeText("Select piece");
    QAbstractButton* pButtonYes = msgBox.addButton(tr("Queen"), QMessageBox::YesRole);
    msgBox.addButton(tr("Rook"), QMessageBox::YesRole);
    msgBox.addButton(tr("Bishop"), QMessageBox::YesRole);
    msgBox.addButton(tr("Knight"), QMessageBox::YesRole);
    msgBox.exec();
    if (msgBox.clickedButton()->text() == "Queen")
        _game->setPiece_selected_from_promotion(Queen);
    else if (msgBox.clickedButton()->text() == "Rook")
        _game->setPiece_selected_from_promotion(Rook);
    else if (msgBox.clickedButton()->text() == "Bishop")
        _game->setPiece_selected_from_promotion(Bishop);
    else if (msgBox.clickedButton()->text() == "Knight")
        _game->setPiece_selected_from_promotion(Knight);
}

void MainWindow::highlightCheck(){
    _info_label->setText("Check");
    for (auto square: _square_widgets){
        if (square->id() == QString::fromStdString(_game->_square_under_check)){
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

void MainWindow::performPawnPromotionGraphically(Move move){
    _info_label->setText("Pawn was promoted");
    QString square = QString::fromStdString(move._destination_square);
    PieceWidget *pieceToRemove;
    for (auto piece: _piece_widgets)
        if (piece->piece_position() == square)
            pieceToRemove = piece;
    QPixmap pixmapOfPiece;
    QString denotation = move._colour_performing_move == White ? "white" : "black";
    if (_game->getPiece_selected_from_promotion() == Queen)
        pixmapOfPiece = move._colour_performing_move == White ? _graphics_info._white_queen : _graphics_info._black_queen;
    else if (_game->getPiece_selected_from_promotion() == Rook)
        pixmapOfPiece = move._colour_performing_move == White ? _graphics_info._white_rook : _graphics_info._black_rook;
    else if (_game->getPiece_selected_from_promotion() == Bishop)
        pixmapOfPiece = move._colour_performing_move == White ? _graphics_info._white_bishop : _graphics_info._black_bishop;
    else if (_game->getPiece_selected_from_promotion() == Knight)
        pixmapOfPiece = move._colour_performing_move == White ? _graphics_info._white_knight : _graphics_info._black_knight;
    removePieceGraphically(pieceToRemove);
    addPieceGraphically(pixmapOfPiece, square, denotation);
}

void MainWindow::removeEnPassantCapturedPieceGraphically(Move move){
    int rowFrom = _game->IndicesFromSquareID(move._origin_square).first;
    int colTo = _game->IndicesFromSquareID(move._destination_square).second;
    QString square = QString::fromStdString(_game->squareIDFromIndices(rowFrom, colTo));
    PieceWidget *pieceToRemove;
    for (auto piece: _piece_widgets)
        if (piece->piece_position() == square)
            pieceToRemove = piece;
    removePieceGraphically(pieceToRemove);
}

void MainWindow::removeCapturedPieceGraphically(Move move){
    QString square = QString::fromStdString(move._destination_square);
    PieceWidget *pieceToRemove;
    for (auto piece: _piece_widgets)
        if (piece->piece_position() == square)
            pieceToRemove = piece;
    removePieceGraphically(pieceToRemove);
}

void MainWindow::moveRookForCastlingGraphically(Move move){
    QString rookOrigin;
    QString rookDestination;
    if (move._colour_performing_move == White){
        rookOrigin = move._move_type == LongCastle ? "a1" : "h1";
        rookDestination = move._move_type == LongCastle ? "d1" : "f1";
    }
    else{
        rookOrigin = move._move_type == LongCastle ? "a8" : "h8";
        rookDestination = move._move_type == LongCastle ? "d8" : "f8";
    }
    PieceWidget *pieceToMove;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == rookOrigin){
            pieceToMove = piece;
        }
    }
    QPixmap pixmapOfPiece = pieceToMove->piece_pixmap();
    QString denotation = pieceToMove->denotation();
    removePieceGraphically(pieceToMove);
    addPieceGraphically(pixmapOfPiece, rookDestination, denotation);
}

void MainWindow::startDraggingMove(QString originSquare){
    if (_game->_is_game_over)
        return;
    bool squareContainsPiece = false;
    bool liftingOpponentPiece = false;
    for (auto piece: _piece_widgets){

        if (piece->piece_position() == originSquare){
            if ((piece->denotation() == "white" && _game->getUser_colour() == Black) ||
                    (piece->denotation() == "black" && _game->getUser_colour() == White)){
                //liftingOpponentPiece = true;
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
        _piece_widget_of_moved_from_square->setPiece_pixmap(_pixmap_of_dragged_piece);
        _piece_widget_of_moved_from_square->populateWithPixmap();
        _piece_widget_currently_dragged = nullptr;
        highlightPreviousMove();
        if (_game->getCurrent_state()->_white_king_is_in_check || _game->getCurrent_state()->_black_king_is_in_check)
            highlightCheck();
        return;
    }
    if (!completeMove(_currently_hovered_square)){
        qDebug() << "move was not legal";
        _dragging_move_in_progress = false;
        removePieceGraphically(_piece_widget_currently_dragged);
        _piece_widget_of_moved_from_square->setPiece_pixmap(_pixmap_of_dragged_piece);
        _piece_widget_of_moved_from_square->populateWithPixmap();
        _piece_widget_currently_dragged = nullptr;
        highlightPreviousMove();
        if (_game->getCurrent_state()->_white_king_is_in_check || _game->getCurrent_state()->_black_king_is_in_check)
            highlightCheck();
        return;
    }
    _legal_destination_squares_for_origin_square.clear();
    _dragging_move_in_progress = false;
    removePieceGraphically(_piece_widget_currently_dragged);
    PieceWidget *pieceToMove;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == _move_in_progress_origin_square){
            pieceToMove = piece;
        }
    }
    QString denotation = pieceToMove->denotation();
    removePieceGraphically(pieceToMove);
    addPieceGraphically(_pixmap_of_dragged_piece, _currently_hovered_square, denotation);
    _piece_widget_currently_dragged = nullptr;
    if (_move_was_promotion)
        performPawnPromotionGraphically(_promotion_move);
    qDebug() << "completed dragging move to: " + _currently_hovered_square;
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

void MainWindow::setPlayerWhite(){
    qDebug() << "playing as white";
    _user_is_white = true;
    _set_white_button->setEnabled(false);
    _set_black_button->setEnabled(true);
    restartGame(White);
    initiateBoardSquaresUI();
    initiatePiecesGraphically();
}

void MainWindow::setPlayerBlack(){
    qDebug() << "playing as black";
    _user_is_white = false;
    _set_white_button->setEnabled(true);
    _set_black_button->setEnabled(false);
    restartGame(Black);
    initiateBoardSquaresUI();
    initiatePiecesGraphically();
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
                    square = new SquareWidget(colsFromIndex.at(j-1) + QString::number(i+1), _graphics_info._black_square, "black");
                    _board_grid_layout->addWidget(square, abs(8-i), j);
                    _square_widgets.append(square);
                }
                else{
                    square = new SquareWidget(colsFromIndex.at(j-1) + QString::number(i+1), _graphics_info._white_square, "white");
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
                    square = new SquareWidget(colsFromIndex.at(j) + QString::number(i+1), _graphics_info._black_square, "black");
                    _board_grid_layout->addWidget(square, i+1, abs(8-j));
                    _square_widgets.append(square);
                }
                else{
                    square = new SquareWidget(colsFromIndex.at(j) + QString::number(i+1), _graphics_info._white_square, "white");
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
            QLabel *letter1 = new QLabel(colsFromIndex.at(i-1));
            QLabel *letter2 = new QLabel(colsFromIndex.at(i-1));
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
            QLabel *letter1 = new QLabel(colsFromIndex.at(abs(8-i)));
            QLabel *letter2 = new QLabel(colsFromIndex.at(abs(8-i)));
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
