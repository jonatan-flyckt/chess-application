#ifndef ZOBRISTHASHER_H
#define ZOBRISTHASHER_H

#define RANDOM_SEED 89
#define RAND_MAX 0xffffffffffffffff
#define RAND_MIN 1

#include "chessstructs.h"
#include <iostream>
#include <random>


class ZobristHasher{
public:

    ZobristHasher();

    ULL generateHashForPosition(BitBoard board, CastlingInfo castlingInfo, Colour turnToMove, ULL enPassantSquare);

    ULL generateRandomNumber();

    map<Piece, vector<ULL>> generateRandomNumbersForPieces();

    map<Piece, vector<ULL>> _piece_numbers;

    ULL _black_to_move_number;

    vector<ULL> _castling_rights_numbers;

    vector<ULL> _en_passant_file_numbers;

    int _current_random_index;

    Piece _white_pawn = Piece(White, Pawn);
    Piece _white_rook = Piece(White, Rook);
    Piece _white_knight = Piece(White, Knight);
    Piece _white_bishop = Piece(White, Bishop);
    Piece _white_queen = Piece(White, Queen);
    Piece _white_king = Piece(White, King);
    Piece _black_pawn = Piece(Black, Pawn);
    Piece _black_rook = Piece(Black, Rook);
    Piece _black_knight = Piece(Black, Knight);
    Piece _black_bishop = Piece(Black, Bishop);
    Piece _black_queen = Piece(Black, Queen);
    Piece _black_king = Piece(Black, King);

};

#endif // ZOBRISTHASHER_H
