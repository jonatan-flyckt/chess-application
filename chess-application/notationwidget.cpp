#include "notationwidget.h"


NotationWidget::NotationWidget(State *state){
    _state = state;
    _move = _state->_move_to_state;
    _move_notation = QString::fromStdString(_move._algebraic_notation);
    this->setText(_move_notation);
    setToolTipText();
}

void NotationWidget::setToolTipText(){
    _tooltip_text = "test tooltip";
    //"click to go see position",move number, move colour, move piece, move type, move was check, move ended game/game result
}

void NotationWidget::enterEvent(QEvent *e){
    qDebug() << "entered notation: " << _move_notation;
}

void NotationWidget::leaveEvent(QEvent *e){
    qDebug() << "left notation: " << _move_notation;
}

void NotationWidget::mousePressEvent(QMouseEvent *e){
    emit clickedSendState(_state);
}

State *NotationWidget::state() const{
    return _state;
}
