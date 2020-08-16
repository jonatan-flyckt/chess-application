#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#define MAX_DEPTH 2
#define INFINITY_POS 1000000000
#define INFINITY_NEG -1000000000

#include "chessstructs.h"
#include "chessrules.h"
#include <random>
#include <algorithm>
#include <vector>
#include <utility>

using namespace std;

struct MiniMaxNode{
    pair<Move, float> _move_eval_pair;
    State *_state;
    vector<MiniMaxNode*> _children;
    int _depth_of_node;
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
    float heuristicValueForState(State *state);
    float simpleMaterialEvaluation(State *state);
    Move makeRandomMove(State *state);
    void addAllPromotionSelections(State *state);

    Move miniMax(State *state, Colour engineColour);
    pair<Move, float> alphaBeta(MiniMaxTree *tree, MiniMaxNode *node, int depth, bool maximisingPlayer);
private:

    ChessRules _rules;

    map<int, MiniMaxTree*> *_move_number_minimax_tree_map;

};

#endif // CHESSENGINE_H
