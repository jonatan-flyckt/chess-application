#include "graphicsinfo.h"

GraphicsInfo::GraphicsInfo(){
    setGraphicsFromPath("Standard", White);
}

void GraphicsInfo::setGraphicsFromPath(QString themeName, Colour playerColour){
    _current_theme_name = themeName;

    _current_graphics_path = ":/images/images/" + _current_theme_name + "/";

    if (!QFileInfo::exists(_current_graphics_path + "pieces/white_king.png")) {
        _current_graphics_path = ":/images/images/Standard/";
        qDebug() << "Graphics path does not exist.";
    }

    qDebug() << "Setting graphics theme path to: " << _current_graphics_path;

    setSquareGraphics(playerColour);


    /*
    _legal_move_highlight_black.load(graphicsPath+"legal_move_highlight_black.png");
    _legal_move_highlight_white.load(graphicsPath+"legal_move_highlight_white.png");
    _check_highlight_black.load(graphicsPath+"check_highlight_black.png");
    _check_highlight_white.load(graphicsPath+"check_highlight_white.png");
    _misc_highlight_black.load(graphicsPath+"misc_square_highlight_black.png");
    _misc_highlight_white.load(graphicsPath+"misc_square_highlight_white.png");

    _white_square.load(graphicsPath+"white_square.png");
    _black_square.load(graphicsPath+"black_square.png");
    */




    _white_king.load(_current_graphics_path+"pieces/white_king.png");
    _white_queen.load(_current_graphics_path+"pieces/white_queen.png");
    _white_rook.load(_current_graphics_path+"pieces/white_rook.png");
    _white_bishop.load(_current_graphics_path+"pieces/white_bishop.png");
    _white_knight.load(_current_graphics_path+"pieces/white_knight.png");
    _white_pawn.load(_current_graphics_path+"pieces/white_pawn.png");

    _black_king.load(_current_graphics_path+"pieces/black_king.png");
    _black_queen.load(_current_graphics_path+"pieces/black_queen.png");
    _black_rook.load(_current_graphics_path+"pieces/black_rook.png");
    _black_bishop.load(_current_graphics_path+"pieces/black_bishop.png");
    _black_knight.load(_current_graphics_path+"pieces/black_knight.png");
    _black_pawn.load(_current_graphics_path+"pieces/black_pawn.png");

    _header_font = QFont("Arial", HEADER_FONT_SIZE, QFont::Bold);
    _fen_font = QFont("Arial", FEN_FONT_SIZE);
    _info_font = QFont("Arial", INFO_FONT_SIZE, QFont::Bold);
}

void GraphicsInfo::setSquareGraphics(Colour playerColour){
    //Sets square graphics depending on player viewing perspective
    QString squarePath = _current_graphics_path + "squares/" + ((playerColour == White) ? "white_perspective/" : "black_perspective/");
    qDebug() << "Setting square graphics with path: " << squarePath;

    for (int row = 1; row <= 8;  row++){
        for (int colID = 0; colID <= 7;  colID++){
            string colName = _cols_from_index[colID];
            QString squareName = QString::fromStdString(colName + std::to_string(row));
            qDebug() << squareName;

            QPixmap baseSquarePixmap;
            baseSquarePixmap.load(squarePath+"base_squares/" + squareName + ".png");
            _base_square_map.insert(squareName, baseSquarePixmap);

            QPixmap legalSquarePixmap;
            legalSquarePixmap.load(squarePath+"legal_move_highlight_squares/" + squareName + ".png");
            _legal_move_highlight_square_map.insert(squareName, legalSquarePixmap);

            QPixmap checkSquarePixmap;
            checkSquarePixmap.load(squarePath+"check_highlight_squares/" + squareName + ".png");
            _check_highlight_square_map.insert(squareName, checkSquarePixmap);

            QPixmap miscSquarePixmap;
            miscSquarePixmap.load(squarePath+"misc_highlight_squares/" + squareName + ".png");
            _misc_highlight_square_map.insert(squareName, miscSquarePixmap);
        }
    }
}

