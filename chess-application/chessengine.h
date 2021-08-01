#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#define MAX_DEPTH 3
#define INFINITY_POS 1000000000
#define INFINITY_NEG -1000000000

//#include <chessstructs.h>
#include "chessrules.h"
#include <random>
#include <algorithm>
#include <vector>
#include <utility>
#include <QElapsedTimer>
#include <QDebug>

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
};

class ChessEngine: public BitBoardUtils
{
public:
    ChessEngine();
    ~ChessEngine();
    Move selectMoveFromState(State *state, Colour engineColour);
    float heuristicValueForState(State *state);
    float simpleMaterialEvaluation(State *state);
    float simpleOpeningEvaluation(State *state);
    Move makeRandomMove(State *state);

    Move miniMax(State *state, Colour engineColour);
    pair<Move, float> alphaBeta(MiniMaxTree *tree, MiniMaxNode *node, int depth, float alpha, float beta, bool maximisingPlayer);
    int countBitsInBoard(ULL board);
    int increaseInSearchDepth(State *state);
private:

    ChessRules _rules;

    map<int, MiniMaxTree*> *_move_number_minimax_tree_map;

    QElapsedTimer _move_generation_timer;

    float _accumulated_move_generation_time;

    QElapsedTimer _heuristic_evaluation_timer;

    float _accumulated_heuristic_evaluation_time;

    QElapsedTimer _alpha_beta_timer;

    float _accumulated_alpha_beta_time;

};

#endif // CHESSENGINE_H
