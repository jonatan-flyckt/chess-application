#include "squarewidget.h"

SquareWidget::SquareWidget(QString id, QPixmap pixmap, QString denotation){
    _inner_layout = new QHBoxLayout(this);
    _inner_layout->setSpacing(0);
    _inner_layout->setContentsMargins(0,0,0,0);
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
    //qDebug() << "entered square";
    emit signalCurrentHovered(_id);
    if (getDraggingMoveReadyToCompleteStatus())
        emit signalCompleteDraggingMove();

}

void SquareWidget::leaveEvent(QEvent *event){ //User stopped hovering square
    //qDebug() << "bounds: " << emit getBoundsOfBoard();
    //qDebug() << "mouse pos: " << mapToGlobal(QCursor::pos());
    if (stayedInSameSquareDuringDraggingMove()){
        emit signalCurrentHovered(_id);
    }
    else
        emit signalCurrentHovered("");
}

void SquareWidget::mousePressEvent(QMouseEvent *ev){
    if (!getClickingMoveStatus()){
        emit signalStartClickingMove(_id);
    }
    else{
        emit signalCompleteClickingMove(_id);
    }
}

void SquareWidget::mouseMoveEvent(QMouseEvent *ev){
    if (!getDraggingMoveStatus())
        emit signalStartDraggingMove(_id);
    else{
        QPoint point = QCursor::pos();
        emit signalMovePieceWidget(point);
    }
}

void SquareWidget::mouseReleaseEvent(QMouseEvent *ev){
    qDebug() << "mouse was released in square";
    if (!getDraggingMoveStatus())
        return;
    QString origin = emit getMoveOriginSquare();
    QString hovered = emit getHoveredSquare();
    //qDebug() << "origin: " << origin;
    //qDebug() << "hovered: " << hovered;
    if (hovered == origin){ //Widget was released on origin square
        emit signalCompleteDraggingMove();
        return;
    }
    emit signalDraggingMoveReadyToComplete();
}

void SquareWidget::changePixmap(QPixmap newPixmap){
    _square_pixmap = newPixmap;
    this->populateWithPixmap();
}

bool SquareWidget::stayedInSameSquareDuringDraggingMove(){
    QRect widgetRect = this->geometry();
    widgetRect.moveTopLeft(this->parentWidget()->mapToGlobal(widgetRect.topLeft()));
    return widgetRect.contains(QCursor::pos());
}

QString SquareWidget::id() const{
    return _id;
}

void SquareWidget::setId(const QString &id){
    _id = id;
}

QBoxLayout *SquareWidget::inner_layout() const{
    return _inner_layout;
}

void SquareWidget::setInner_layout(QBoxLayout *inner_layout){
    _inner_layout = inner_layout;
}

QBoxLayout *SquareWidget::getInner_layout() const
{
    return _inner_layout;
}

QString SquareWidget::getDenotation() const
{
    return _denotation;
}

void SquareWidget::setDenotation(const QString &denotation)
{
    _denotation = denotation;
}
