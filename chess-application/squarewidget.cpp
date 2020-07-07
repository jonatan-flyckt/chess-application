#include "squarewidget.h"

SquareWidget::SquareWidget(QString id, QPixmap pixmap, QString denotation){
    _id = id;
    _square_pixmap = pixmap;
    _denotation = denotation;
    populateWithPixmap();
}

void SquareWidget::populateWithPixmap(){
    this->setPixmap(_square_pixmap.scaled(5, 5, Qt::KeepAspectRatio));
    this->setScaledContents(true);
}

void SquareWidget::enterEvent(QEvent *event){ //User started hovering square
    qDebug() << "Entered " << _id << ": " << _denotation;
    emit signalCurrentHovered(_id);
}

void SquareWidget::leaveEvent(QEvent *event){ //User stopped hovering square
    qDebug() << "Left " << _id << ": " << _denotation;
    emit signalCurrentHovered("");
}
