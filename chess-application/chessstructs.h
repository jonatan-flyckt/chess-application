#ifndef STRUCTS_H
#define STRUCTS_H

#endif // STRUCTS_H

#include <string>
#include <vector>
#include <map>
#include <tuple>

using namespace std;

enum Colour{White, Black};

enum Piece{Pawn, Rook, Knight, Bishop, Queen, King};

struct Move{
    Colour _colour;
    bool _long_castle;
    bool _short_castle;
    bool _en_passant;
    bool _promotion;
    Piece _piece;
    string _origin_square;
    string _destination_square;
    string _algebraic_notation;
    int _move_number;
};

struct State{
    bool _white_castled;
    bool _black_castled;
    Colour _colour_to_move;
    Move _move_to_state;
    Move _move_from_state;
    vector<Move> _legal_moves_from_state;
    State *_previous_state;
    int _moves_without_capture;

    //Bitboard notation:
    //First dimension: each piece type
    //Second dimension: rows 1 through 8
    //Third dimension: columns a through h
    bool _bit_board[12][8][8];
    map<int, tuple<Colour, Piece>> _bitboard_piece_map ={
        {0, make_tuple(White, Pawn)},
        {1, make_tuple(White, Rook)},
        {2, make_tuple(White, Knight)},
        {3, make_tuple(White, Bishop)},
        {4, make_tuple(White, Queen)},
        {5, make_tuple(White, King)},
        {6, make_tuple(Black, Pawn)},
        {7, make_tuple(Black, Rook)},
        {8, make_tuple(Black, Knight)},
        {9, make_tuple(Black, Bishop)},
        {10, make_tuple(Black, Queen)},
        {11, make_tuple(Black, King)}
    };

    string _fen_notation;
};
