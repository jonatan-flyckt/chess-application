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

    QPixmap legal_move_highlight_white;
    QPixmap legal_move_highlight_black;
    QPixmap check_highlight_white;
    QPixmap check_highlight_black;

    QPixmap black_square;
    QPixmap white_square;

    QPixmap white_king;
    QPixmap white_queen;
    QPixmap white_rook;
    QPixmap white_bishop;
    QPixmap white_knight;
    QPixmap white_pawn;

    QPixmap black_king;
    QPixmap black_queen;
    QPixmap black_rook;
    QPixmap black_bishop;
    QPixmap black_knight;
    QPixmap black_pawn;

private:

};

#endif // GRAPHICSINFO_H
