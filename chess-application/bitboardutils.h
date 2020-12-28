#ifndef BITBOARDUTILS_H
#define BITBOARDUTILS_H

#include <vector>
#include <map>
#include "utils.h"
#include <cstring>
#include <iostream>

enum BishopDirections{NW, NE, SE, SW};

enum RookDirections{N, E, S, W};

class BitBoardUtils
{
public:

    ULL*  generateKnightMoveSet();

    ULL*  generateKingMoveSet();

    ULL*  generateRookMoveSet();

    map<BishopDirections, ULL>*  generateBishopMoveSet();

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

    ULL getRookMovesForIndex(int index, ULL allPieces);

    ULL getBishopMovesForSquare(int index, ULL allPieces);

    string _square_from_index[64] = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};

    map<string, int> _index_from_square{
        {"a1", 0}, {"b1", 1},  {"c1", 2}, {"d1", 3}, {"e1", 4}, {"f1", 5}, {"g1", 6}, {"h1", 7},
        {"a2", 8}, {"b2", 9},  {"c2", 10}, {"d2", 11}, {"e2", 12}, {"f2", 13}, {"g2", 14}, {"h2", 15},
        {"a3", 16}, {"b3", 17}, {"c3", 18}, {"d3", 19}, {"e3", 20}, {"f3", 21}, {"g3", 22}, {"h3", 23},
        {"a4", 24}, {"b4", 25}, {"c4", 26}, {"d4", 27}, {"e4", 28}, {"f4", 29}, {"g4", 30}, {"h4", 31},
        {"a5", 32}, {"b5", 33}, {"c5", 34}, {"d5", 35}, {"e5", 36}, {"f5", 37}, {"g5", 38}, {"h5", 39},
        {"a6", 40}, {"b6", 41}, {"c6", 42}, {"d6", 43}, {"e6", 44}, {"f6", 45}, {"g6", 46}, {"h6", 47},
        {"a7", 48}, {"b7", 49}, {"c7", 50}, {"d7", 51}, {"e7", 52}, {"f7", 53}, {"g7", 54}, {"h7", 55},
        {"a8", 56}, {"b8", 57}, {"c8", 58}, {"d8", 59}, {"e8", 60}, {"f8", 61}, {"g8", 62}, {"h8", 63}
    };

    const int _magic_bit_table[64] ={
        63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19,
        34, 61, 29, 2, 51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0,
        35, 62, 31, 40, 4, 49, 5, 52, 26, 60, 6, 23, 44, 46, 27, 56,
        16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28, 58, 20, 37, 17, 36, 8
    };

    const int _most_significant_bit_pos[32] = {0, 1, 28, 2, 29, 14, 24, 3,
      30, 22, 20, 15, 25, 17, 4, 8, 31, 27, 13, 23, 21, 19,
      16, 7, 26, 12, 18, 6, 11, 5, 10, 9};

    ULL mirrorHorizontal (ULL x);

    ULL mirrorVertical(ULL x);

    ULL* _knight_move_set = generateKnightMoveSet();

    ULL* _king_move_set = generateKingMoveSet();

    ULL* _rook_move_set = generateRookMoveSet();

    ULL* _white_pawn_move_set = generatePawnMoveSet(White);

    ULL* _black_pawn_move_set = generatePawnMoveSet(Black);

    ULL* _white_pawn_capture_set = generatePawnCaptureSet(White);

    ULL* _black_pawn_capture_set = generatePawnCaptureSet(Black);

    map<Piece, ULL> _starting_bitboard = generateStartingPosition();

    ULL* _bit_masks = generateBitMasks().first;

    ULL* _bit_masks_complement = generateBitMasks().second;


    map<BishopDirections, ULL>* _bishop_square_attack_rays = generateBishopMoveSet();

    int getIndexOfLeastSignificantBit(ULL bitboard);
    int getIndexOfMostSignificantBit(unsigned int v);
};

#endif // BITBOARDUTILS_H

