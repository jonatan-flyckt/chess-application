#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "chessstructs.h"
#include <random>
#include <algorithm>

using namespace std;

class ChessEngine
{
public:
    ChessEngine();
    ~ChessEngine();
    Move selectMoveFromState(State *state, Colour engineColour);
    int evaluateState(State *state, Colour engineColour);
    Move miniMax(State *state, Colour engineColour);
};

#endif // CHESSENGINE_H
