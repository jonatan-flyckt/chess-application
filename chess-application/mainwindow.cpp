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

    initiateUIComponents();

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
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

void MainWindow::initiatePiecesGraphically(){
    for (int i = 0; i < 8; i++){
        addPieceGraphically(_graphics_info.black_pawn, colsFromIndex.at(i) + "7");
        addPieceGraphically(_graphics_info.white_pawn, colsFromIndex.at(i) + "2");
    }

    addPieceGraphically(_graphics_info.white_rook, "a1");
    addPieceGraphically(_graphics_info.white_knight, "b1");
    addPieceGraphically(_graphics_info.white_bishop, "c1");
    addPieceGraphically(_graphics_info.white_queen, "d1");
    addPieceGraphically(_graphics_info.white_king, "e1");
    addPieceGraphically(_graphics_info.white_bishop, "f1");
    addPieceGraphically(_graphics_info.white_knight, "g1");
    addPieceGraphically(_graphics_info.white_rook, "h1");

    addPieceGraphically(_graphics_info.black_rook, "a8");
    addPieceGraphically(_graphics_info.black_knight, "b8");
    addPieceGraphically(_graphics_info.black_bishop, "c8");
    addPieceGraphically(_graphics_info.black_queen, "d8");
    addPieceGraphically(_graphics_info.black_king, "e8");
    addPieceGraphically(_graphics_info.black_bishop, "f8");
    addPieceGraphically(_graphics_info.black_knight, "g8");
    addPieceGraphically(_graphics_info.black_rook, "h8");
}

void MainWindow::highlightLegalSquares(){

}

void MainWindow::removeLegalSquaresHighlight(){

}

void MainWindow::addPieceGraphically(QPixmap pieceGraphic, QString squareID){
    SquareWidget *squareToPlacePieceOn;
    for (SquareWidget *square: _square_widgets){
        if (square->id() == squareID)
            squareToPlacePieceOn = square;
    }
    PieceWidget *piece = new PieceWidget();
    piece->setPiece_pixmap(pieceGraphic);
    piece->populateWithPixmap();
    piece->setPiece_position(squareID);
    squareToPlacePieceOn->inner_layout()->addWidget(piece);
    _piece_widgets.append(piece);
}

void MainWindow::removePieceGraphically(PieceWidget *piece){
    int index = _piece_widgets.indexOf(piece);
    _piece_widgets.remove(index);
    delete piece;
}

void MainWindow::initiateUIComponents(){
    _main_grid_layout = new QGridLayout();
    _ui->centralwidget->setLayout(_main_grid_layout);
    _board_grid_layout = new QGridLayout();

    _board_grid_layout->setSpacing(0);

    _board_aspect_ratio_widget = new BoardAspectRatioWidget(_board_grid_layout, 500, 500, _ui->centralwidget);

    _main_grid_layout->setColumnMinimumWidth(BOARD_GRID_COL, 500);
    _main_grid_layout->setRowMinimumHeight(BOARD_GRID_ROW, 500);
    _main_grid_layout->setColumnStretch(BOARD_GRID_COL, 100);
    _main_grid_layout->setRowStretch(BOARD_GRID_ROW, 100);

    _info_label = new QLabel();
    QFont f( "Arial", 15);
    _info_label->setFont(f);
    _info_label->setStyleSheet("color: rgb(255, 0, 0)");
    _main_grid_layout->addWidget(_info_label, 0, 0);

    _set_white_button = new QPushButton(this);
    _set_white_button->setText("Play as white");
    connect(_set_white_button, SIGNAL(clicked()), this, SLOT(setPlayerWhite()));
    _main_grid_layout->addWidget(_set_white_button, 2, 2);
    _set_white_button->setEnabled(false);

    _set_black_button = new QPushButton(this);
    _set_black_button->setText("Play as black");
    connect(_set_black_button, SIGNAL(clicked()), this, SLOT(setPlayerBlack()));
    _main_grid_layout->addWidget(_set_black_button, 2, 3);


    _main_grid_layout->addWidget(_board_aspect_ratio_widget, BOARD_GRID_ROW, BOARD_GRID_COL);

    initiateBoardSquaresUI();

    initiatePiecesGraphically();
}

void MainWindow::setInfoMessage(QString message){
    _info_label->setText(message);
}

void MainWindow::setCurrentHovered(QString id){
    _currently_hovered_square = id;
    qDebug() << "current hovered set to" << _currently_hovered_square;
}

void MainWindow::startClickingMove(QString originSquare){
    bool squareContainsPiece = false;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == originSquare)
            squareContainsPiece = true;
    }
    if (!squareContainsPiece)
        return;
    _clicking_move_in_progress = true;
    _dragging_move_in_progress = false;
    _move_in_progress_origin_square = originSquare;
    _legal_destination_squares = {"d4", "d5", "e4", "e5"};
    qDebug() << "started clicking move from: " + originSquare;
}

void MainWindow::completeClickingMove(QString destinationSquare){
    if (!_legal_destination_squares.contains(destinationSquare)){
        qDebug() << "move was not legal";
        _clicking_move_in_progress = false;
        return;
    }
    PieceWidget *pieceToMove;
    QPixmap pieceGraphic;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == _move_in_progress_origin_square){
            pieceToMove = piece;
            pieceGraphic = pieceToMove->piece_pixmap();
        }
    }
    addPieceGraphically(pieceGraphic, destinationSquare);
    removePieceGraphically(pieceToMove);
    _clicking_move_in_progress = false;
    _move_in_progress_origin_square = "";
    qDebug() << "completed clicking move to: " + destinationSquare;
}

void MainWindow::startDraggingMove(QString originSquare){
    bool squareContainsPiece = false;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == originSquare)
            squareContainsPiece = true;
    }
    if (!squareContainsPiece)
        return;
    _clicking_move_in_progress = false;
    _dragging_move_in_progress = true;
    _move_in_progress_origin_square = originSquare;

    QPixmap widgetGraphic;
    QSize widgetSize;
    PieceWidget *pieceToMove;
    for (auto piece: _piece_widgets){
        if (piece->piece_position() == originSquare){
            widgetSize = piece->size();
            widgetGraphic = piece->piece_pixmap();
            pieceToMove = piece;
            break;
        }
    }

    _piece_widget_currently_dragged = new PieceWidget();
    _piece_widget_currently_dragged->setPiece_pixmap(widgetGraphic);
    _piece_widget_currently_dragged->setPiece_position("");
    _piece_widget_currently_dragged->populateWithPixmap();
    _piece_widget_currently_dragged->resize(widgetSize.width(), widgetSize.height());
    _piece_widget_currently_dragged->setFrameStyle(Qt::FramelessWindowHint);
    _piece_widget_currently_dragged->show();

    _piece_widgets.append(_piece_widget_currently_dragged);
    //removePieceGraphically(pieceToMove);

    qDebug() << "started dragging move from: " + originSquare;
}

void MainWindow::setDraggingMoveReadyToComplete(){
    _dragging_move_ready_to_complete = true;
}

void MainWindow::completeDraggingMove(){
    _dragging_move_ready_to_complete = false;
    if (!_legal_destination_squares.contains(_currently_hovered_square)){
        qDebug() << "move was not legal";
        _dragging_move_in_progress = false;
        removePieceGraphically(_piece_widget_currently_dragged);
        _piece_widget_currently_dragged = nullptr;
        return;
    }
    _dragging_move_in_progress = false;
    removePieceGraphically(_piece_widget_currently_dragged);
    _piece_widget_currently_dragged = nullptr;
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

void MainWindow::movePieceWidget(QPoint mousePos){
    if (_piece_widget_currently_dragged == nullptr)
        return;
    _piece_widget_currently_dragged->move(mousePos);
}

void MainWindow::setPlayerWhite(){
    qDebug() << "playing as white";
    _user_is_white = true;
    _set_white_button->setEnabled(false);
    _set_black_button->setEnabled(true);
    initiateBoardSquaresUI();
    initiatePiecesGraphically();
}

void MainWindow::setPlayerBlack(){
    qDebug() << "playing as black";
    _user_is_white = false;
    _set_white_button->setEnabled(true);
    _set_black_button->setEnabled(false);
    initiateBoardSquaresUI();
    initiatePiecesGraphically();
}

void MainWindow::initiateBoardSquaresUI(){
    clearBoardUI();
    addColAndRowHeaders();
    for (int i = 0; i < 8; i++){
        if (_user_is_white){
            for (int j = 8; j > 0; j--){
                SquareWidget *square;
                if ((i+j) % 2 == 0){
                    square = new SquareWidget(colsFromIndex.at(j-1) + QString::number(i+1), _graphics_info.black_square, "black");
                    _board_grid_layout->addWidget(square, abs(8-i), j);
                    _square_widgets.append(square);
                }
                else{
                    square = new SquareWidget(colsFromIndex.at(j-1) + QString::number(i+1), _graphics_info.white_square, "white");
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
                    square = new SquareWidget(colsFromIndex.at(j) + QString::number(i+1), _graphics_info.black_square, "black");
                    _board_grid_layout->addWidget(square, i+1, abs(8-j));
                    _square_widgets.append(square);
                }
                else{
                    square = new SquareWidget(colsFromIndex.at(j) + QString::number(i+1), _graphics_info.white_square, "white");
                    _board_grid_layout->addWidget(square, i+1, abs(8-j));
                    _square_widgets.append(square);
                }
                connectSquareToSignals(square);
            }
        }
    }
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
}

void MainWindow::addColAndRowHeaders(){
    for (int i = 1; i < 9; i++){
        QFont f( "Arial", 15, QFont::Bold);
        if (_user_is_white){
            QLabel *number1 = new QLabel(QString::number(abs(9-i)) + " ");
            QLabel *number2 = new QLabel(" " + QString::number(abs(9-i)));
            number1->setFont(f);
            number2->setFont(f);
            _board_grid_layout->addWidget(number1, i, 0, Qt::AlignRight);
            _board_grid_layout->addWidget(number2, i, 9, Qt::AlignLeft);
            QLabel *letter1 = new QLabel(colsFromIndex.at(i-1));
            QLabel *letter2 = new QLabel(colsFromIndex.at(i-1));
            letter1->setFont(f);
            letter2->setFont(f);
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
            number1->setFont(f);
            number2->setFont(f);
            _board_grid_layout->addWidget(number1, i, 0, Qt::AlignRight);
            _board_grid_layout->addWidget(number2, i, 9, Qt::AlignLeft);
            QLabel *letter1 = new QLabel(colsFromIndex.at(abs(8-i)));
            QLabel *letter2 = new QLabel(colsFromIndex.at(abs(8-i)));
            letter1->setFont(f);
            letter2->setFont(f);
            _board_grid_layout->addWidget(letter1, 0, i, Qt::AlignHCenter | Qt::AlignBottom);
            _board_grid_layout->addWidget(letter2, 9, i, Qt::AlignHCenter | Qt::AlignTop);
            _board_header_labels.append(number1);
            _board_header_labels.append(number2);
            _board_header_labels.append(letter1);
            _board_header_labels.append(letter2);
        }

    }
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
