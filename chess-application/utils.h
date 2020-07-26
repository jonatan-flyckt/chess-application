#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "chessstructs.h"

using namespace std;

string stringFromDifficulty(Difficulty dif);

Difficulty difficultyFromString(string str);

class Utils
{
public:
    Utils();

};

#endif // UTILS_H
