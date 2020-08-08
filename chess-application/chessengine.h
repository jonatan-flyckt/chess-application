#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "chessstructs.h"
#include "chessgame.h"
#include <random>
#include <algorithm>

using namespace std;

class ChessEngine
{
public:
    ChessEngine();
    ~ChessEngine();
    Move selectMoveFromState(ChessGame *game, State *state, Colour engineColour);
    int evaluateState(State *state, Colour engineColour);
    Move miniMax(State *state, Colour engineColour);
    int simpleMaterialEvaluation(State *state);
    Move makeRandomMove(State *state);
};

#endif // CHESSENGINE_H
