#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow){
    _ui->setupUi(this);

    initiateUIComponents();

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

MainWindow::~MainWindow(){
    delete _ui;
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

    _main_grid_layout->addWidget(new QLabel("hejsan"), 0, 0);
    _main_grid_layout->addWidget(new QLabel("hejsan 2"), 2, 2);

    _main_grid_layout->addWidget(_board_aspect_ratio_widget, BOARD_GRID_ROW, BOARD_GRID_COL);
    initiateBoardSquaresUI();
}

void MainWindow::initiateBoardSquaresUI(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if ((i+j) % 2 == 0){
                QLabel *w_label = new QLabel();
                w_label->setPixmap(_graphics_info.w_square.scaled(5, 5, Qt::KeepAspectRatio));
                w_label->setScaledContents(true);
                _board_grid_layout->addWidget(w_label, i, j);
            }
            else{
                QLabel *b_label = new QLabel();
                b_label->setPixmap(_graphics_info.b_square.scaled(5, 5, Qt::KeepAspectRatio));
                b_label->setScaledContents(true);
                _board_grid_layout->addWidget(b_label, i, j);
            }
        }
    }
}

