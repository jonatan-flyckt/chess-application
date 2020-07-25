#include "notationwidget.h"

#include <QApplication>

NotationWidget::NotationWidget(State *state){
    _state = state;
    _move = _state->_move_to_state;
    _move_notation = QString::fromStdString(_move._algebraic_notation);
    this->setText(_move_notation);
    setToolTipText();
    qApp->setStyleSheet("QToolTip { color: white; border: 1px solid white; }");
    this->setToolTip(_tooltip_text);
}

void NotationWidget::setToolTipText(){
    _tooltip_text = "";
    _tooltip_text.append("Move number:\t\t" + QString::number((_state->_move_to_state._move_number+1)/2) + "\n");
    _tooltip_text.append("Piece:\t\t\t");
    _tooltip_text.append(_move._colour_performing_move == White ? "White " : "Black ");
    if (_move._piece._type == Pawn)
        _tooltip_text.append("Pawn\n");
    else if (_move._piece._type == Rook)
        _tooltip_text.append("Rook\n");
    else if (_move._piece._type == Bishop)
        _tooltip_text.append("Bishop\n");
    else if (_move._piece._type == Knight)
        _tooltip_text.append("Knight\n");
    else if (_move._piece._type == Queen)
        _tooltip_text.append("Queen\n");
    else if (_move._piece._type == King)
        _tooltip_text.append("King\n");
    _tooltip_text.append("Move type:\t\t");
    if (_move._move_type == Standard)
        _tooltip_text.append("Standard\n");
    else if (_move._move_type == Capture)
        _tooltip_text.append("Capture\n");
    else if (_move._move_type == LongCastle)
        _tooltip_text.append("Queenside castling\n");
    else if (_move._move_type == ShortCastle)
        _tooltip_text.append("Kingside castling\n");
    else if (_move._move_type == EnPassant)
        _tooltip_text.append("En passant\n");
    else if (_move._move_type == Promotion)
        _tooltip_text.append("Promotion\n");
    else if (_move._move_type == PromotionCapture)
        _tooltip_text.append("Promotion capture\n");

    _tooltip_text.append("Origin square:\t\t" + QString::fromStdString(_move._origin_square) + "\n");
    _tooltip_text.append("Destination square:\t" + QString::fromStdString(_move._destination_square) + "\n");

    if (_state->_white_king_is_in_check || _state->_black_king_is_in_check){
        if (_state->_is_game_over)
            _tooltip_text.append("\n\t---Check mate---");
        else
            _tooltip_text.append("\n\t\t-Check-");
    }
    else if (_state->_is_game_over){
        _tooltip_text.append("\n\t---Game over---");
    }
}

void NotationWidget::mousePressEvent(QMouseEvent *e){
    emit clickedSendState(_state);
}

State *NotationWidget::state() const{
    return _state;
}
