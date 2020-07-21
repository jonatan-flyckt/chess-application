#include "boardaspectratiowidget.h"

BoardAspectRatioWidget::BoardAspectRatioWidget(QGridLayout *boardLayout, float width, float height, QWidget *parent):
QWidget(parent), arWidth(width), arHeight(height){
    layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->addItem(new QSpacerItem(0, 0));
    layout->addLayout(boardLayout);
    layout->addItem(new QSpacerItem(0, 0));
}

//Make sure the board keeps a square aspect ratio if the window is resized
void BoardAspectRatioWidget::resizeEvent(QResizeEvent *event){

    float thisAspectRatio = (float)event->size().width() / event->size().height();
    int widgetStretch, outerStretch;

    if (thisAspectRatio > (arWidth/arHeight)){ // too wide
        layout->setDirection(QBoxLayout::LeftToRight);
        widgetStretch = height() * (arWidth/arHeight);
        outerStretch = (width() - widgetStretch) / 2 + 0.5;
    }
    else{ // too tall
        layout->setDirection(QBoxLayout::TopToBottom);
        widgetStretch = width() * (arHeight/arWidth);
        outerStretch = (height() - widgetStretch) / 2 + 0.5;
    }

    layout->setStretch(0, outerStretch);
    layout->setStretch(1, widgetStretch);
    layout->setStretch(2, outerStretch);


    for(auto square: emit getSquareWidgets()){
        square->setSquareScaleFactor(std::min(this->width()/10, this->height()/10));
        square->populateWithPixmap();
    }

}
