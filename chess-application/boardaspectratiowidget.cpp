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

    layout->setDirection(QBoxLayout::TopToBottom);
    if (thisAspectRatio > (arWidth/arHeight)){//Too wide, set spacers on the sides
        layout->setDirection(QBoxLayout::LeftToRight);
        widgetStretch = height() * (arWidth/arHeight);
        outerStretch = (width() - widgetStretch) / 2 + 0.5;
    }
    else{//Too tall, set spacers on the top and bottom
        layout->setDirection(QBoxLayout::TopToBottom);
        widgetStretch = width() * (arHeight/arWidth);
        outerStretch = (height() - widgetStretch) / 2 + 0.5;
    }

    layout->setStretch(0, outerStretch);
    layout->setStretch(1, widgetStretch);
    layout->setStretch(2, outerStretch);

    for(auto square: emit getSquareWidgets()){
        square->setSquareScaleFactor(std::min(parentWidget()->size().height()/10, this->width()/10));
        square->populateWithPixmap();
    }

}
