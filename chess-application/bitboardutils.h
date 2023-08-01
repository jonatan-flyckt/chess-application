#ifndef BITBOARDUTILS_H
#define BITBOARDUTILS_H

#include <vector>
#include <map>
#include <unordered_map>
#include "utils.h"
#include <cstring>
#include <iostream>

enum BishopDirections{NW, NE, SE, SW};

enum RookDirections{N, E, S, W};

class BitBoardUtils
{
public:

    ULL getBit(ULL bitboard, int index){return (bitboard & (1ULL << index));}
    ULL setBit(ULL bitboard, int index){return (bitboard |= (1ULL << index));}
    ULL popBit(ULL bitboard, int index){return (getBit(bitboard, index) ? (bitboard ^= (1ULL << index)) : 0);}

    ULL*  generateKnightMoveSet();

    ULL*  generateKingMoveSet();

    unordered_map<BishopDirections, ULL>*  generateBishopMoveSet();

    unordered_map<RookDirections, ULL>*  generateRookMoveSet();

    ULL*  generatePawnMoveSet(Colour colour);

    ULL*  generatePawnCaptureSet(Colour colour);

    ULL* generateFilledUpToMasks();

    ULL* generateFilledDownToMasks();

    unordered_map<Piece, ULL> generateStartingPosition();

    pair<ULL*, ULL*> generateBitMasks();

    int countBitsInBoard(ULL board);

    int popLeastSignificantBitFromBoard(ULL *board);

    vector<int> getIndicesOfBitsInBoard(ULL board);

    vector<int> bitVectorFromULL(ULL board);

    void printBoardOnOneRow(ULL board);

    void printBoard(ULL board);

    int getIndexOfLeastSignificantBit(const ULL &bitboard);

    int getIndexOfMostSignificantBit(ULL bitboard);

    ULL mirrorHorizontal (ULL x);

    ULL mirrorVertical(ULL x);

    ULL generateLargeCentreMask();

    ULL generateMediumCentreMask();

    ULL generateSmallCentreMask();

    unordered_map<ULL, int> generateSinglePieceIndexMap();

    unordered_map<ULL, vector<int>> generateTwoPieceIndexMap();

    const string _square_from_index[64] = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};

    unordered_map<string, int> _index_from_square{
        {"a1", 0}, {"b1", 1},  {"c1", 2}, {"d1", 3}, {"e1", 4}, {"f1", 5}, {"g1", 6}, {"h1", 7},
        {"a2", 8}, {"b2", 9},  {"c2", 10}, {"d2", 11}, {"e2", 12}, {"f2", 13}, {"g2", 14}, {"h2", 15},
        {"a3", 16}, {"b3", 17}, {"c3", 18}, {"d3", 19}, {"e3", 20}, {"f3", 21}, {"g3", 22}, {"h3", 23},
        {"a4", 24}, {"b4", 25}, {"c4", 26}, {"d4", 27}, {"e4", 28}, {"f4", 29}, {"g4", 30}, {"h4", 31},
        {"a5", 32}, {"b5", 33}, {"c5", 34}, {"d5", 35}, {"e5", 36}, {"f5", 37}, {"g5", 38}, {"h5", 39},
        {"a6", 40}, {"b6", 41}, {"c6", 42}, {"d6", 43}, {"e6", 44}, {"f6", 45}, {"g6", 46}, {"h6", 47},
        {"a7", 48}, {"b7", 49}, {"c7", 50}, {"d7", 51}, {"e7", 52}, {"f7", 53}, {"g7", 54}, {"h7", 55},
        {"a8", 56}, {"b8", 57}, {"c8", 58}, {"d8", 59}, {"e8", 60}, {"f8", 61}, {"g8", 62}, {"h8", 63}
    };

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

    const int _most_significant_bit_table[64] = {
        0, 47,  1, 56, 48, 27,  2, 60,
       57, 49, 41, 37, 28, 16,  3, 61,
       54, 58, 35, 52, 50, 42, 21, 44,
       38, 32, 29, 23, 17, 11,  4, 62,
       46, 55, 26, 59, 40, 36, 15, 53,
       34, 51, 20, 43, 31, 22, 10, 45,
       25, 39, 14, 33, 19, 30,  9, 24,
       13, 18,  8, 12,  7,  6,  5, 63
    };

    const ULL* _knight_move_set = generateKnightMoveSet();

    const ULL* _king_move_set = generateKingMoveSet();

    const ULL* _white_pawn_move_set = generatePawnMoveSet(White);

    const ULL* _black_pawn_move_set = generatePawnMoveSet(Black);

    const ULL* _white_pawn_capture_set = generatePawnCaptureSet(White);

    const ULL* _black_pawn_capture_set = generatePawnCaptureSet(Black);

    unordered_map<Piece, ULL> _starting_bitboard = generateStartingPosition();

    unordered_map<ULL, int> _single_piece_board_index_map = generateSinglePieceIndexMap();

    unordered_map<ULL, vector<int>> _two_piece_board_index_map = generateTwoPieceIndexMap();

    const ULL* _bit_masks = generateBitMasks().first;

    const ULL* _bit_masks_complement = generateBitMasks().second;

    const ULL* _filled_up_to_masks = generateFilledUpToMasks();

    const ULL* _filled_down_to_masks = generateFilledDownToMasks();

    unordered_map<BishopDirections, ULL>* _bishop_square_attack_rays = generateBishopMoveSet();

    unordered_map<RookDirections, ULL>* _rook_square_attack_rays = generateRookMoveSet();

    const ULL _large_centre_mask = generateLargeCentreMask();

    const ULL _medium_centre_mask = generateMediumCentreMask();

    const ULL _small_centre_mask = generateSmallCentreMask();

};

#endif // BITBOARDUTILS_H

