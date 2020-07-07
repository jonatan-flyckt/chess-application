#ifndef PIECEWIDGET_H
#define PIECEWIDGET_H

#include <QPixmap>
#include <QWidget>

class PieceWidget : public QWidget{

public:

    PieceWidget();

private:

    QPixmap _piece_graphic;
    QString _piece_position;

};

#endif // PIECEWIDGET_H
