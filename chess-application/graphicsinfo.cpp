#include "graphicsinfo.h"

GraphicsInfo::GraphicsInfo(){
    setGraphicsFromPath("");
}

void GraphicsInfo::setGraphicsFromPath(QString path){
    QString graphicsPath;
    QFileInfo checkFile(path);
    bool dirExists = (checkFile.exists() && checkFile.isDir());
    if (!dirExists)
        graphicsPath = ":/images/images/placeholders/";
    else
        graphicsPath = path;

    _legal_move_highlight_black.load(graphicsPath+"legal_move_highlight_black.png");
    _legal_move_highlight_white.load(graphicsPath+"legal_move_highlight_white.png");
    _check_highlight_black.load(graphicsPath+"check_highlight_black.png");
    _check_highlight_white.load(graphicsPath+"check_highlight_white.png");
    _misc_highlight_black.load(graphicsPath+"misc_square_highlight_black.png");
    _misc_highlight_white.load(graphicsPath+"misc_square_highlight_white.png");

    _white_square.load(graphicsPath+"white_square.png");
    _black_square.load(graphicsPath+"black_square.png");

    _white_king.load(graphicsPath+"white_king.png");
    _white_queen.load(graphicsPath+"white_queen.png");
    _white_rook.load(graphicsPath+"white_rook.png");
    _white_bishop.load(graphicsPath+"white_bishop.png");
    _white_knight.load(graphicsPath+"white_knight.png");
    _white_pawn.load(graphicsPath+"white_pawn.png");

    _black_king.load(graphicsPath+"black_king.png");
    _black_queen.load(graphicsPath+"black_queen.png");
    _black_rook.load(graphicsPath+"black_rook.png");
    _black_bishop.load(graphicsPath+"black_bishop.png");
    _black_knight.load(graphicsPath+"black_knight.png");
    _black_pawn.load(graphicsPath+"black_pawn.png");

    _header_font = QFont("Arial", HEADER_FONT_SIZE, QFont::Bold);
}
