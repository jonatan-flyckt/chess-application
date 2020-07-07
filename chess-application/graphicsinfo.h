#ifndef GRAPHICSINFO_H
#define GRAPHICSINFO_H

#include <QPixmap>
#include <QFileInfo>
#include <QDebug>

class GraphicsInfo
{
public:
    GraphicsInfo();

    void setGraphicsFromPath(QString path);


    QPixmap b_square;
    QPixmap w_square;

    QPixmap w_king;
    QPixmap w_queen;
    QPixmap w_rook;
    QPixmap w_bishop;
    QPixmap w_knight;
    QPixmap w_pawn;

    QPixmap b_king;
    QPixmap b_queen;
    QPixmap b_rook;
    QPixmap b_bishop;
    QPixmap b_knight;
    QPixmap b_pawn;

private:

};

#endif // GRAPHICSINFO_H
