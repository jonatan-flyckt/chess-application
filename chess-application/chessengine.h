#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#define MAX_DEPTH 4
#define INFINITY_POS 1000000000
#define INFINITY_NEG -1000000000
#define WHITE_WIN_EVAL 100000000 //Must be less than infinity pos
#define BLACK_WIN_EVAL -100000000 //Must be larger than ininity neg


#include "chessrules.h"
#include <random>
#include <algorithm>
#include <vector>
#include <utility>
#include <QDebug>
#include <chrono>

struct MiniMaxNode{
    pair<Move, float> _move_eval_pair;
    State *_state;
    vector<MiniMaxNode*> _children;
    int _depth_of_node;
    float _heuristic_value;
};

struct MiniMaxNodeComparator{
    bool descending;

    MiniMaxNodeComparator(bool descending) : descending(descending) {}

    bool operator()(const MiniMaxNode* node1, const MiniMaxNode* node2) const {
        return descending ? node1->_heuristic_value > node2->_heuristic_value : node1->_heuristic_value < node2->_heuristic_value;
    }
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
    float fastSimpleHeuristicValueForState(State *state);
    float simpleMaterialEvaluation(State *state);
    float simpleOpeningEvaluation(State *state);
    Move makeRandomMove(State *state);

    Move miniMax(State *state, Colour engineColour);
    pair<Move, float> alphaBeta(MiniMaxTree *tree, MiniMaxNode *node, int depth, float alpha, float beta, bool maximisingPlayer);
    int countBitsInBoard(ULL board);

    void insertSorted(vector<MiniMaxNode*>& _children, MiniMaxNode* newNode, bool descending);
private:

    ChessRules _rules;

    unordered_map<int, MiniMaxTree*> *_move_number_minimax_tree_map;

    float _accumulated_move_generation_time;
    float _accumulated_state_generation_time;
    float _accumulated_heuristic_evaluation_time;

};

#endif // CHESSENGINE_H
