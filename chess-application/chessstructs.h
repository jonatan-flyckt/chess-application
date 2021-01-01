#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>
#include <map>
#include <utility>

//#include "utils.h"

typedef uint64_t ULL;

using namespace std;

enum Colour{White, Black};

enum PieceType{Pawn, Rook, Knight, Bishop, Queen, King, NoPiece};

enum MoveType{Standard, Capture, LongCastle, ShortCastle, EnPassant, Promotion, PromotionCapture};

enum Difficulty{Easy, Normal, Hard, FakeLast};

struct Piece{
    Piece(Colour colour, PieceType type){_colour = colour; _type = type;};
    Piece(){};
    Colour _colour;
    PieceType _type;

    bool operator==(const Piece &o) const{
        return _colour == o._colour && _type == o._type;
    }

    bool operator<(const Piece &o) const{
        return _colour < o._colour || (_colour == o._colour && _type < o._type);
    }
};

struct Move{
    Move(Colour colour, Piece piece, string originSquare,
         string destinationSquare, /*int originIndex,
         int destinationIndex,*/ int moveNumber,
         MoveType moveType = Standard, PieceType promotionSelection = Queen){
        _colour_performing_move = colour;
        _piece = piece;
        _origin_square = originSquare;
        _destination_square = destinationSquare;
        /*
        _origin_index = originIndex;
        _desination_index = destinationIndex;
        */
        _move_number = moveNumber;
        _move_type = moveType;
        _promotion_selection = promotionSelection;
    };
    Move(){};
    Colour _colour_performing_move;
    Piece _piece;
    MoveType _move_type;
    string _origin_square;
    string _destination_square;
    /*
    int _origin_index;
    int _desination_index;
    */
    string _algebraic_notation;
    int _move_number;
    PieceType _promotion_selection;
};

struct CastlingInfo{
    bool _white_castled = false;
    bool _white_king_has_moved = false;
    bool _white_long_rook_has_moved = false;
    bool _white_short_rook_has_moved = false;
    bool _black_castled = false;
    bool _black_king_has_moved = false;
    bool _black_long_rook_has_moved = false;
    bool _black_short_rook_has_moved = false;
};

struct BitBoard{
    BitBoard(){
    }

    ULL _white_pawns;
    ULL _white_rooks;
    ULL _white_knights;
    ULL _white_bishops;
    ULL _white_queens;
    ULL _white_king;

    ULL _black_pawns;
    ULL _black_rooks;
    ULL _black_knights;
    ULL _black_bishops;
    ULL _black_queens;
    ULL _black_king;

    ULL _all_white_pieces;
    ULL _all_black_pieces;
    ULL _all_pieces;

    ULL _en_passant_square;
};

struct State{
    State(){};
    Colour _colour_to_move;
    Move _move_to_state;
    Move _move_from_state;
    vector<Move> _legal_moves_from_state;
    State *_previous_state = nullptr;
    State *_next_state = nullptr;
    CastlingInfo _castling_info;
    int _moves_without_capture_or_pawn_advancement;

    bool _is_game_over = false;
    bool _white_king_is_in_check = false;
    bool _black_king_is_in_check = false;
    string _square_under_check = "";

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


    BitBoard _bit_board;


    string _fen_notation;




    bool _is_draw = false;
    bool _white_won = false;
    bool _black_won = false;
    string _game_over_reason;
    map<string, int> *_state_seen_count;
};

#endif // STRUCTS_H
