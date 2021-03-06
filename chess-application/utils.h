#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "chessstructs.h"
#include <tuple>
#include <iostream>


typedef uint64_t ULL;

using namespace std;

static const vector<string> _cols_from_index{"a", "b", "c", "d", "e", "f", "g", "h"};

string stringFromDifficulty(Difficulty dif);

Difficulty difficultyFromString(string str);

string squareIDFromIndices(int row, int col);

pair<int, int> IndicesFromSquareID(string square);

uint32_t bitAbs(uint32_t num);

vector<string> splitString(string str, string delimiter);


#endif // UTILS_H
