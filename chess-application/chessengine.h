#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "chessstructs.h"
//#include "chessgame.h"
#include "chessrules.h"
#include <random>
#include <algorithm>
#include <vector>

using namespace std;

struct MiniMaxNode{
    float _evaluation;
    State *_state;
    vector<Move> _legal_moves;
    vector<MiniMaxNode*> _children;
};

struct MiniMaxTree{
    MiniMaxNode *_starting_node;
    int _max_depth;
    Move _best_move;
    float _alpha;
    float _beta;
};

class ChessEngine
{
public:
    ChessEngine();
    ~ChessEngine();
    Move selectMoveFromState(State *state, Colour engineColour);
    int evaluateState(State *state, Colour engineColour);
    Move miniMax(State *state, Colour engineColour);
    int simpleMaterialEvaluation(State *state);
    Move makeRandomMove(State *state);

private:
    ChessRules _rules;
};

#endif // CHESSENGINE_H
