#ifndef STRUCTS_H
#define STRUCTS_H



#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

enum Colour{White, Black};

enum PieceType{Pawn, Rook, Knight, Bishop, Queen, King};

struct Piece{
    Piece(Colour colour, PieceType type){_colour = colour; _type = type;};
    Piece(){};
    Colour _colour;
    PieceType _type;
};

struct Move{
    Move(Colour colour, Piece piece, string originSquare,
         string destinationSquare, int moveNumber,
         bool longCastle = false, bool shortCastle = false, bool enPassant = false,
         bool promotion = false, bool isCapture = false){
        _colour_performing_move = colour;
        _piece = piece;
        _origin_square = originSquare;
        _destination_square = destinationSquare;
        _move_number = moveNumber;
        _long_castle = longCastle;
        _short_castle = shortCastle;
        _en_passant = enPassant;
        _promotion = promotion;
        _is_capture = isCapture;
    };
    Move(){};
    Colour _colour_performing_move;
    Piece _piece;
    bool _long_castle;
    bool _short_castle;
    bool _en_passant;
    bool _promotion;
    bool _is_capture;
    string _origin_square;
    string _destination_square;
    string _algebraic_notation;
    int _move_number;
};


struct State{
    State(){};
    bool _white_castled;
    bool _black_castled;
    Colour _colour_to_move;
    Move _move_to_state;
    Move _move_from_state;
    vector<Move> _legal_moves_from_state;
    State *_previous_state;
    int _moves_without_capture;

    bool _is_game_over;
    bool _is_white_check;
    bool _is_black_check;

    //Nnumber of moves to reach this state (white + black)
    int _number_of_moves;

    //outer vector: vector of rows
    vector<vector<Piece*>> _board{
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
    };

    //TODO: Determine if bitboard is needed
    //Bitboard notation:
    //First dimension: each piece type
    //Second dimension: rows 1 through 8
    //Third dimension: columns a through h
    /*bool _bit_board[12][8][8];
    map<int, pair<Colour, Piece>> _bitboard_piece_map ={
        {0, make_pair(White, Pawn)},
        {1, make_pair(White, Rook)},
        {2, make_pair(White, Knight)},
        {3, make_pair(White, Bishop)},
        {4, make_pair(White, Queen)},
        {5, make_pair(White, King)},
        {6, make_pair(Black, Pawn)},
        {7, make_pair(Black, Rook)},
        {8, make_pair(Black, Knight)},
        {9, make_pair(Black, Bishop)},
        {10, make_pair(Black, Queen)},
        {11, make_pair(Black, King)}
    };*/

    string _fen_notation;
};

#endif // STRUCTS_H
