#ifndef BITBOARDUTILS_H
#define BITBOARDUTILS_H

#include <vector>
#include <map>
#include "utils.h"
#include <cstring>
#include <iostream>

class BitBoardUtils
{
public:

    ULL*  generateKnightMoveSet();

    ULL*  generateKingMoveSet();

    ULL*  generateRookMoveSet();

    ULL*  generateBishopMoveSet();

    ULL*  generateQueenMoveSet();

    ULL*  generatePawnMoveSet(Colour colour);

    ULL*  generatePawnCaptureSet(Colour colour);

    map<Piece, ULL> generateStartingPosition();

    pair<ULL*, ULL*> generateBitMasks();

    int countBitsInBoard(ULL board);

    int popLeastSignificantBitFromBoard(ULL *board);

    vector<int> getIndicesOfBitsInBoard(ULL board);

    vector<int> bitVectorFromULL(ULL board);

    void printBoardOnOneRow(ULL board);

    void printBoard(ULL board);

    string _square_from_index[64] = {
        "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8",
        "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8",
        "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8",
        "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8",
        "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
        "g1", "g2", "g3", "g4", "g5", "g6", "g7", "g8",
        "h1", "h2", "h3", "h4", "h5", "h6", "h7", "h8"};

    const int _magic_bit_table[64] ={
        63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19,
        34, 61, 29, 2, 51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0,
        35, 62, 31, 40, 4, 49, 5, 52, 26, 60, 6, 23, 44, 46, 27, 56,
        16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28, 58, 20, 37, 17, 36, 8
    };

    ULL* _knight_move_set = generateKnightMoveSet();

    ULL* _king_move_set = generateKingMoveSet();

    ULL* _rook_move_set = generateRookMoveSet();

    ULL* _bishop_move_set = generateBishopMoveSet();

    ULL* _queen_move_set = generateQueenMoveSet();

    ULL* _white_pawn_move_set = generatePawnMoveSet(White);

    ULL* _black_pawn_move_set = generatePawnMoveSet(Black);

    ULL* _white_pawn_capture_set = generatePawnCaptureSet(White);

    ULL* _black_pawn_capture_set = generatePawnCaptureSet(Black);

    map<Piece, ULL> _starting_bitboard = generateStartingPosition();

    ULL* _bit_masks = generateBitMasks().first;

    ULL* _bit_masks_complement = generateBitMasks().second;

};

#endif // BITBOARDUTILS_H

