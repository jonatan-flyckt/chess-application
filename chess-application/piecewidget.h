#ifndef PIECEWIDGET_H
#define PIECEWIDGET_H

#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QDebug>

class PieceWidget : public QLabel{

public:

    PieceWidget(QString denotation);

    void populateWithPixmap();

    QString piece_position() const;
    void setPiece_position(const QString &piece_position);

    QPixmap piece_pixmap() const;
    void setPiece_pixmap(const QPixmap &piece_pixmap);

    QString denotation() const;
    void setDenotation(const QString &denotation);

private:

    QString _piece_position;
    QPixmap _piece_pixmap;
    QString _denotation;
};

#endif // PIECEWIDGET_H
