#include "squarewidget.h"

SquareWidget::SquareWidget(QString id, QPixmap pixmap, QString denotation){
    _inner_layout = new QHBoxLayout(this);
    _id = id;
    _square_pixmap = pixmap;
    _denotation = denotation;
    populateWithPixmap();
    this->setLayout(_inner_layout);
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

QString SquareWidget::id() const
{
    return _id;
}

void SquareWidget::setId(const QString &id)
{
    _id = id;
}

QBoxLayout *SquareWidget::inner_layout() const
{
    return _inner_layout;
}

void SquareWidget::setInner_layout(QBoxLayout *inner_layout)
{
    _inner_layout = inner_layout;
}
