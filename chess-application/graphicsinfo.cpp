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

    w_square.load(graphicsPath+"w_square.png");
    b_square.load(graphicsPath+"b_square.png");

    w_king.load(graphicsPath+"white_K.png");
    w_queen.load(graphicsPath+"white_Q.png");
    w_rook.load(graphicsPath+"white_R.png");
    w_bishop.load(graphicsPath+"white_B.png");
    w_knight.load(graphicsPath+"white_Kn.png");
    w_pawn.load(graphicsPath+"white_P.png");

    b_king.load(graphicsPath+"black_K.png");
    b_queen.load(graphicsPath+"black_Q.png");
    b_rook.load(graphicsPath+"black_R.png");
    b_bishop.load(graphicsPath+"black_B.png");
    b_knight.load(graphicsPath+"black_Kn.png");
    b_pawn.load(graphicsPath+"black_P.png");
}
