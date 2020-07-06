#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow){
    _ui->setupUi(this);



    initiateUIComponents();
}

MainWindow::~MainWindow(){
    delete _ui;
}

void MainWindow::initiateUIComponents(){
    _board_layout = _ui->board_grid_layout;
    initiateBoardSquaresUI();

    _ui->board_widget->setLayout(_board_layout);


}

void MainWindow::initiateBoardSquaresUI(){
    QPixmap b_pixmap(":/../../chess-images/placeholders/b_square.png");
    QPixmap w_pixmap(":/../../chess-images/placeholders/w_square.png");
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){

            if ((i+j) % 2 == 0){
                QLabel *w_label = new QLabel();
                w_label->setPixmap(w_pixmap);
                //_board_layout->addWidget(w_label, i, j);
                _board_layout->addWidget(new QLabel("white"), i, j);
            }
            else{
                QLabel *b_label = new QLabel();
                b_label->setPixmap(b_pixmap);
                //_board_layout->addWidget(b_label, i, j);
                _board_layout->addWidget(new QLabel("black"), i, j);
            }
        }
    }
}

