#ifndef ZOBRISTHASHER_H
#define ZOBRISTHASHER_H

#define RANDOM_SEED 89

#include "chessstructs.h"
#include <iostream>
#include <random>


class ZobristHasher{
public:

    ZobristHasher();

    ULL generateHashForPosition(BitBoard board, CastlingInfo castlingInfo, Colour turnToMove, ULL enPassantSquare);

    ULL generateRandomNumber();

    map<Piece, ULL*> generateRandomNumbersForPieces();

    map<Piece, ULL*> _piece_numbers;

    ULL _black_to_move_number;

    vector<ULL> _castling_rights_numbers;

    vector<ULL> _en_passant_file_numbers;

    int _current_random_index;

};

#endif // ZOBRISTHASHER_H
