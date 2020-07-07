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

BoardAspectRatioWidget::BoardAspectRatioWidget(QGridLayout *boardLayout, float width, float height, QWidget *parent):
QWidget(parent), arWidth(width), arHeight(height){
    layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    // add spacer, then your widget, then spacer
    layout->addItem(new QSpacerItem(0, 0));
    //layout->addWidget(widget);
    layout->addLayout(boardLayout);
    layout->addItem(new QSpacerItem(0, 0));
}

void BoardAspectRatioWidget::resizeEvent(QResizeEvent *event){

    float thisAspectRatio = (float)event->size().width() / event->size().height();
    int widgetStretch, outerStretch;

    if (thisAspectRatio > (arWidth/arHeight)) // too wide
    {
        layout->setDirection(QBoxLayout::LeftToRight);
        widgetStretch = height() * (arWidth/arHeight); // i.e., my width
        outerStretch = (width() - widgetStretch) / 2 + 0.5;
    }
    else // too tall
    {
        layout->setDirection(QBoxLayout::TopToBottom);
        widgetStretch = width() * (arHeight/arWidth); // i.e., my height
        outerStretch = (height() - widgetStretch) / 2 + 0.5;
    }

    layout->setStretch(0, outerStretch);
    layout->setStretch(1, widgetStretch);
    layout->setStretch(2, outerStretch);
}



void MainWindow::initiateUIComponents(){
    _main_grid_layout = new QGridLayout();

    _ui->centralwidget->setLayout(_main_grid_layout);




    _board_grid_layout = new QGridLayout();
    _board_aspect_ratio_widget = new BoardAspectRatioWidget(_board_grid_layout, 500, 500, _ui->centralwidget);
    //_board_aspect_ratio_widget->setLayout(_board_grid_layout);


    qDebug() << _board_grid_layout->heightForWidth(1);


    _main_grid_layout->setColumnMinimumWidth(BOARD_GRID_COL, 500);
    _main_grid_layout->setRowMinimumHeight(BOARD_GRID_ROW, 500);
    _main_grid_layout->setColumnStretch(BOARD_GRID_COL, 100);
    _main_grid_layout->setRowStretch(BOARD_GRID_ROW, 100);


    _main_grid_layout->addWidget(new QLabel("hejsan"), 0, 0);

    _main_grid_layout->addWidget(new QLabel("hejsan 2"), 2, 2);

    _main_grid_layout->addWidget(_board_aspect_ratio_widget, BOARD_GRID_ROW, BOARD_GRID_COL);
    //_main_grid_layout->addLayout(_board_grid_layout, BOARD_GRID_ROW, BOARD_GRID_COL);

    initiateBoardSquaresUI();


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
                _board_grid_layout->addWidget(new QLabel("white"), i, j);
            }
            else{
                QLabel *b_label = new QLabel();
                b_label->setPixmap(b_pixmap);
                //_board_layout->addWidget(b_label, i, j);
                _board_grid_layout->addWidget(new QLabel("black"), i, j);
            }
        }
    }
}

