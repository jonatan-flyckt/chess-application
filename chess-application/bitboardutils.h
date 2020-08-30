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

    map<int, ULL>  generateKnightAttackSet();

    map<int, ULL>  generateKingAttackSet();

    map<int, ULL>  generateRookAttackSet();

    map<int, ULL>  generateBishopAttackSet();

    map<int, ULL>  generateQueenAttackSet();

    map<int, ULL>  generatePawnAttackSet(Colour colour);

    map<Piece, ULL> generateStartingPosition();

    vector<int> bitVectorFromULL(ULL board);

    void printBoardOnOneRow(ULL board);

    void printBoard(ULL board);

    map<int, string> _square_from_index = {
        {0, "a1"}, {1, "a2"}, {2, "a3"}, {3, "a4"}, {4, "a5"}, {5, "a6"}, {6, "a7"}, {7, "a8"},
        {8, "b1"}, {9, "b2"}, {10, "b3"}, {11, "b4"}, {12, "b5"}, {13, "b6"}, {14, "b7"}, {15, "b8"},
        {16, "c1"}, {17, "c2"}, {18, "c3"}, {19, "c4"}, {20, "c5"}, {21, "c6"}, {22, "c7"}, {23, "c8"},
        {24, "d1"}, {25, "d2"}, {26, "d3"}, {27, "d4"}, {28, "d5"}, {29, "d6"}, {30, "d7"}, {31, "d8"},
        {32, "e1"}, {33, "e2"}, {34, "e3"}, {35, "e4"}, {36, "e5"}, {37, "e6"}, {38, "e7"}, {39, "e8"},
        {40, "f1"}, {41, "f2"}, {42, "f3"}, {43, "f4"}, {44, "f5"}, {45, "f6"}, {46, "f7"}, {47, "f8"},
        {48, "g1"}, {49, "g2"}, {50, "g3"}, {51, "g4"}, {52, "g5"}, {53, "g6"}, {54, "g7"}, {55, "g8"},
        {56, "h1"}, {57, "h2"}, {58, "h3"}, {59, "h4"}, {60, "h5"}, {61, "h6"}, {62, "h7"}, {63, "h8"},
    };

    map<int, ULL> _knight_attack_set = generateKnightAttackSet();

    map<int, ULL> _king_attack_set = generateKingAttackSet();

    map<int, ULL> _rook_attack_set = generateRookAttackSet();

    map<int, ULL> _bishop_attack_set = generateBishopAttackSet();

    map<int, ULL> _queen_attack_set = generateQueenAttackSet();

    map<int, ULL> _white_pawn_attack_set = generatePawnAttackSet(White);

    map<int, ULL> _black_pawn_attack_set = generatePawnAttackSet(Black);

    map<Piece, ULL> _starting_bitboard = generateStartingPosition();

};

#endif // BITBOARDUTILS_H

