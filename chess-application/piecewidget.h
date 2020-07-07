#ifndef PIECEWIDGET_H
#define PIECEWIDGET_H

#include <QPixmap>
#include <QWidget>
#include <QLabel>

class PieceWidget : public QLabel{

public:

    PieceWidget();

    void populateWithPixmap();

    QString piece_position() const;
    void setPiece_position(const QString &piece_position);

    QPixmap piece_pixmap() const;
    void setPiece_pixmap(const QPixmap &piece_pixmap);

private:

    QString _piece_position;
    QPixmap _piece_pixmap;

};

#endif // PIECEWIDGET_H
