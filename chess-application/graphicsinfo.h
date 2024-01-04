#ifndef GRAPHICSINFO_H
#define GRAPHICSINFO_H

#define HEADER_FONT_SIZE 12
#define START_HEADER_FONT_SIZE 21

#define FEN_FONT_SIZE 7

#define INFO_FONT_SIZE 8
#define START_INFO_FONT_SIZE 12

#include <QPixmap>
#include <QFileInfo>
#include <QDebug>
#include <QFont>
#include <chessstructs.h>



class GraphicsInfo
{
public:
    GraphicsInfo();

    void setGraphicsFromPath(QString themeName, Colour playerColour);

    QPixmap _legal_move_highlight_white;
    QPixmap _legal_move_highlight_black;
    QPixmap _check_highlight_white;
    QPixmap _check_highlight_black;
    QPixmap _misc_highlight_white;
    QPixmap _misc_highlight_black;

    QPixmap _black_square;
    QPixmap _white_square;



    QMap<QString, QPixmap> _base_square_map;
    QMap<QString, QPixmap> _legal_move_highlight_square_map;
    QMap<QString, QPixmap> _check_highlight_square_map;
    QMap<QString, QPixmap> _misc_highlight_square_map;




    QPixmap _white_king;
    QPixmap _white_queen;
    QPixmap _white_rook;
    QPixmap _white_bishop;
    QPixmap _white_knight;
    QPixmap _white_pawn;

    QPixmap _black_king;
    QPixmap _black_queen;
    QPixmap _black_rook;
    QPixmap _black_bishop;
    QPixmap _black_knight;
    QPixmap _black_pawn;

    QFont _header_font;
    QFont _fen_font;
    QFont _info_font;

private:

};

#endif // GRAPHICSINFO_H
