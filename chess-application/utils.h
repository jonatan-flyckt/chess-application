#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "chessstructs.h"
#include <tuple>
#include <map>

typedef uint64_t ULL;

using namespace std;

static const vector<string> _cols_from_index{"a", "b", "c", "d", "e", "f", "g", "h"};

string stringFromDifficulty(Difficulty dif);

Difficulty difficultyFromString(string str);

string squareIDFromIndices(int row, int col);

pair<int, int> IndicesFromSquareID(string square);

ULL knightTargetSq(int id);





map<int, ULL> _knight_attack_set = {
    {0, knightTargetSq(0)}, {1, knightTargetSq(1)}, {2, knightTargetSq(2)}, {3, knightTargetSq(3)},
    {4, knightTargetSq(4)}, {5, knightTargetSq(5)}, {6, knightTargetSq(6)}, {7, knightTargetSq(7)},
    {8, knightTargetSq(8)}, {9, knightTargetSq(9)}, {10, knightTargetSq(10)}, {11, knightTargetSq(11)},
    {12, knightTargetSq(12)}, {13, knightTargetSq(13)}, {14, knightTargetSq(14)}, {15, knightTargetSq(15)},
    {16, knightTargetSq(16)}, {17, knightTargetSq(17)}, {18, knightTargetSq(18)}, {19, knightTargetSq(19)},
    {20, knightTargetSq(20)}, {21, knightTargetSq(21)}, {22, knightTargetSq(22)}, {23, knightTargetSq(23)},
    {24, knightTargetSq(24)}, {25, knightTargetSq(25)}, {26, knightTargetSq(26)}, {27, knightTargetSq(27)},
    {28, knightTargetSq(28)}, {29, knightTargetSq(29)}, {30, knightTargetSq(30)}, {31, knightTargetSq(31)},
    {32, knightTargetSq(32)}, {33, knightTargetSq(33)}, {34, knightTargetSq(34)}, {35, knightTargetSq(35)},
    {36, knightTargetSq(36)}, {37, knightTargetSq(37)}, {38, knightTargetSq(38)}, {39, knightTargetSq(39)},
    {40, knightTargetSq(40)}, {41, knightTargetSq(41)}, {42, knightTargetSq(42)}, {43, knightTargetSq(43)},
    {44, knightTargetSq(44)}, {45, knightTargetSq(45)}, {46, knightTargetSq(46)}, {47, knightTargetSq(47)},
    {48, knightTargetSq(48)}, {49, knightTargetSq(49)}, {50, knightTargetSq(50)}, {51, knightTargetSq(51)},
    {52, knightTargetSq(52)}, {53, knightTargetSq(53)}, {54, knightTargetSq(54)}, {55, knightTargetSq(55)},
    {56, knightTargetSq(56)}, {57, knightTargetSq(57)}, {58, knightTargetSq(58)}, {59, knightTargetSq(59)},
    {60, knightTargetSq(60)}, {61, knightTargetSq(61)}, {62, knightTargetSq(62)}, {63, knightTargetSq(63)}
};






#endif // UTILS_H
