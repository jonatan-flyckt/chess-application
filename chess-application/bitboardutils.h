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

    map<string, int> _index_from_square{
        {"a1", 0}, {"a2", 1}, {"a3",2 }, {"a4", 3}, {"a5", 4}, {"a6", 5}, {"a7", 6}, {"a8", 7},
        {"b1", 8}, {"b2", 9}, {"b3", 10}, {"b4", 11}, {"b5", 12}, {"b6", 13}, {"b7", 14}, {"b8", 15},
        {"c1", 16}, {"c2", 17}, {"c3", 18}, {"c4", 19}, {"c5", 20}, {"c6", 21}, {"c7", 22}, {"c8", 23},
        {"d1", 24}, {"d2", 25}, {"d3", 26}, {"d4", 27}, {"d5", 28}, {"d6", 29}, {"d7", 30}, {"d8", 31},
        {"e1", 32}, {"e2", 33}, {"e3", 34}, {"e4", 35}, {"e5", 36}, {"e6", 37}, {"e7", 38}, {"e8", 39},
        {"f1", 40}, {"f2", 41}, {"f3", 42}, {"f4", 43}, {"f5", 44}, {"f6", 45}, {"f7", 46}, {"f8", 47},
        {"g1", 48}, {"g2", 49}, {"g3", 50}, {"g4", 51}, {"g5", 52}, {"g6", 53}, {"g7", 54}, {"g8", 55},
        {"h1", 56}, {"h2", 57}, {"h3", 58}, {"h4", 59}, {"h5", 60}, {"h6", 61}, {"h7", 61}, {"h8", 63}
    };

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

