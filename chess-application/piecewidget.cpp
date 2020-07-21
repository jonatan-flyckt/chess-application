#include "piecewidget.h"

PieceWidget::PieceWidget(QString denotation){
    _denotation = denotation;
}

void PieceWidget::populateWithPixmap(){
    this->setPixmap(_piece_pixmap.scaled(1000, 1000, Qt::KeepAspectRatio));
    this->setScaledContents(true);
}

QString PieceWidget::piece_position() const
{
    return _piece_position;
}

void PieceWidget::setPiece_position(const QString &piece_position)
{
    _piece_position = piece_position;
}

QPixmap PieceWidget::piece_pixmap() const
{
    return _piece_pixmap;
}

void PieceWidget::setPiece_pixmap(const QPixmap &piece_pixmap)
{
    _piece_pixmap = piece_pixmap;
}

QString PieceWidget::denotation() const
{
    return _denotation;
}

void PieceWidget::setDenotation(const QString &denotation)
{
    _denotation = denotation;
}
