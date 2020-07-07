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

}

void MainWindow::addPieceGraphically(){

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
}

void MainWindow::setInfoMessage(QString message){
    _info_label->setText(message);
}

void MainWindow::setCurrentHovered(QString id){
    _currently_hovered_square = id;
}

void MainWindow::setPlayerWhite(){
    qDebug() << "clicked";
    _user_is_white = true;
    _set_white_button->setEnabled(false);
    _set_black_button->setEnabled(true);
    initiateBoardSquaresUI();
}

void MainWindow::setPlayerBlack(){
    qDebug() << "clicked b";
    _user_is_white = false;
    _set_white_button->setEnabled(true);
    _set_black_button->setEnabled(false);
    initiateBoardSquaresUI();
}

void MainWindow::initiateBoardSquaresUI(){
    clearBoardUI();
    addColAndRowHeaders();
    for (int i = 0; i < 8; i++){
        if (_user_is_white){
            for (int j = 8; j > 0; j--){
                if ((i+j) % 2 == 0){
                    SquareWidget *b_square = new SquareWidget(colsFromIndex.at(j-1) + QString::number(i+1), _graphics_info.b_square, "black");
                    connect(b_square, &SquareWidget::signalCurrentHovered, this, &MainWindow::setCurrentHovered);
                    _board_grid_layout->addWidget(b_square, abs(8-i), j);
                    _square_widgets.append(b_square);
                }
                else{
                    SquareWidget *w_square = new SquareWidget(colsFromIndex.at(j-1) + QString::number(i+1), _graphics_info.w_square, "white");
                    connect(w_square, &SquareWidget::signalCurrentHovered, this, &MainWindow::setCurrentHovered);
                    _board_grid_layout->addWidget(w_square, abs(8-i), j);
                    _square_widgets.append(w_square);
                }
            }
        }
        else{
            for (int j = 0; j < 8; j++){
                if ((i+j) % 2 == 0){
                    SquareWidget *b_square = new SquareWidget(colsFromIndex.at(j) + QString::number(i+1), _graphics_info.b_square, "black");
                    connect(b_square, &SquareWidget::signalCurrentHovered, this, &MainWindow::setCurrentHovered);
                    _board_grid_layout->addWidget(b_square, i+1, abs(8-j));
                    _square_widgets.append(b_square);
                }
                else{
                    SquareWidget *w_square = new SquareWidget(colsFromIndex.at(j) + QString::number(i+1), _graphics_info.w_square, "white");
                    connect(w_square, &SquareWidget::signalCurrentHovered, this, &MainWindow::setCurrentHovered);
                    _board_grid_layout->addWidget(w_square, i+1, abs(8-j));
                    _square_widgets.append(w_square);
                }
            }
        }
    }
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
    for (auto label: _board_header_labels)
        delete label;
    _board_header_labels.clear();
    for (auto square: _square_widgets)
        delete square;
    _square_widgets.clear();
}
