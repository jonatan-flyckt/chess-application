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

    white_square.load(graphicsPath+"white_square.png");
    black_square.load(graphicsPath+"black_square.png");

    white_king.load(graphicsPath+"white_knight.png");
    white_queen.load(graphicsPath+"white_queen.png");
    white_rook.load(graphicsPath+"white_rook.png");
    white_bishop.load(graphicsPath+"white_bishop.png");
    white_knight.load(graphicsPath+"white_knight.png");
    white_pawn.load(graphicsPath+"white_pawn.png");

    black_king.load(graphicsPath+"black_king.png");
    black_queen.load(graphicsPath+"black_queen.png");
    black_rook.load(graphicsPath+"black_rook.png");
    black_bishop.load(graphicsPath+"black_bishop.png");
    black_knight.load(graphicsPath+"black_knight.png");
    black_pawn.load(graphicsPath+"black_pawn.png");
}
