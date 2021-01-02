#ifndef ZOBRISTHASHER_H
#define ZOBRISTHASHER_H

#define RANDOM_SEED 89
#define RAND_MAX 0xffffffffffffffff
#define RAND_MIN 1

#include "bitboardutils.h"
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

    const int _least_significant_bit_table[64] ={
        63, 30,  3, 32, 25, 41, 22, 33,
        15, 50, 42, 13, 11, 53, 19, 34,
        61, 29,  2, 51, 21, 43, 45, 10,
        18, 47,  1, 54,  9, 57,  0, 35,
        62, 31, 40,  4, 49,  5, 52, 26,
        60,  6, 23, 44, 46, 27, 56, 16,
         7, 39, 48, 24, 59, 14, 12, 55,
        38, 28, 58, 20, 37, 17, 36,  8
    };
    int popLeastSignificantBitFromBoard(ULL *board);
    vector<int> getIndicesOfBitsInBoard(ULL board);
};

#endif // ZOBRISTHASHER_H
