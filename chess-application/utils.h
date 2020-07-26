#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "chessstructs.h"
#include <tuple>

using namespace std;

const static vector<string> _cols_from_index{"a", "b", "c", "d", "e", "f", "g", "h"};

string stringFromDifficulty(Difficulty dif);

Difficulty difficultyFromString(string str);

string squareIDFromIndices(int row, int col);

pair<int, int> IndicesFromSquareID(string square);

#endif // UTILS_H
