#include "chessengine.h"

ChessEngine::ChessEngine(){
    _move_number_minimax_tree_map = new map<int, MiniMaxTree*>();
}

ChessEngine::~ChessEngine(){
    //TODO: delete all trees individually
    delete _move_number_minimax_tree_map;
}

Move ChessEngine::selectMoveFromState(State *state, Colour engineColour){
    //QMap<int, float> moveIndexEvalValueMap;
    addAllPromotionSelections(state);
    return miniMax(state, engineColour);

    /*for (int i = 0; i < state->_legal_moves_from_state.size(); i++){
        Move move =  state->_legal_moves_from_state.at(i);
        State *resultingState = _rules.getResultingStateFromMove(state, move);
        float eval = simpleMaterialEvaluation(resultingState);
        moveIndexEvalValueMap.insert(i, eval);
    }
    int currentBestIndex = 0;
    int currentBestEval = -1000;
    QMapIterator<int, float> i(moveIndexEvalValueMap);
    while (i.hasNext()) {
        i.next();
        if (currentBestEval == -1000)
            currentBestEval = i.value();
        if (state->_colour_to_move == White){
            if (i.value() > currentBestEval)
                currentBestIndex = i.key();
        }
        else{
            if (i.value() < currentBestEval)
                currentBestIndex = i.key();
        }
    }

    return state->_legal_moves_from_state.at(currentBestIndex);*/

}

Move ChessEngine::miniMax(State *state, Colour engineColour){
    MiniMaxTree *tree = new MiniMaxTree();
    _move_number_minimax_tree_map->insert(pair<int, MiniMaxTree*>(state->_number_of_moves, tree));
    MiniMaxNode *startingNode = new MiniMaxNode();
    pair<Move, float> moveToStateEvalPair;
    moveToStateEvalPair.first = state->_move_to_state;
    moveToStateEvalPair.second = 0;
    startingNode->_move_eval_pair = moveToStateEvalPair;
    startingNode->_state = state;
    startingNode->_depth_of_node = 0;
    tree->_starting_node = startingNode;
    tree->_max_depth = MAX_DEPTH;
    tree->_alpha = INFINITY_NEG;
    tree->_beta = INFINITY_POS;

    pair<Move, float> bestMoveEvalPair = alphaBeta(tree, startingNode, startingNode->_depth_of_node, engineColour == White);

    startingNode->_move_eval_pair.second = bestMoveEvalPair.second;
    tree->_best_move = bestMoveEvalPair.first;
    return tree->_best_move;
}

pair<Move, float> ChessEngine::alphaBeta(MiniMaxTree *tree, MiniMaxNode *node, int depth, bool maximisingPlayer){
    if (depth == tree->_max_depth || node->_state->_is_game_over){
        float value = heuristicValueForState(node->_state);
        pair<Move, float> moveEvalPair;
        moveEvalPair.first = node->_state->_move_to_state;
        moveEvalPair.second = value;
        return moveEvalPair;
    }
    node->_state->_legal_moves_from_state = _rules.getLegalMoves(node->_state);
    //TODO: move ordering
    for (auto legalMove: node->_state->_legal_moves_from_state){
        MiniMaxNode *newNode = new MiniMaxNode();
        newNode->_depth_of_node = depth+1;
        newNode->_state = _rules.getResultingStateFromMove(node->_state, legalMove);
        node->_children.push_back(newNode);
    }

    if (maximisingPlayer){ //Find best move for white (maximise)
        float bestEval = INFINITY_NEG;
        Move bestMove;
        for (auto child: node->_children){
            pair<Move, float> moveEval = alphaBeta(tree, child, depth+1, false);
            child->_move_eval_pair = moveEval;
            if (moveEval.second > bestEval){
                //bestMove = moveEval.first;
                bestMove = child->_state->_move_to_state;
                bestEval = moveEval.second;
            }
            tree->_alpha = max(tree->_alpha, bestEval);
            //Prune tree if possible:
            //if (tree->_alpha >= tree->_beta)
            //    break;
        }
        return pair<Move, float>(bestMove, bestEval);
    }
    else{ //Find best move for black (minimise)
        float bestEval = INFINITY_POS;
        Move bestMove;
        for (auto child: node->_children){
            pair<Move, float> moveEval = alphaBeta(tree, child, depth+1, true);
            child->_move_eval_pair = moveEval;
            if (moveEval.second < bestEval){
                bestMove = child->_state->_move_to_state;
                bestEval = moveEval.second;
            }
            tree->_beta = min(tree->_beta, bestEval);
            //Prune tree if possible:
            //if (tree->_beta <= tree->_alpha)
            //    break;
        }
        return pair<Move, float>(bestMove, bestEval);
    }
}

float ChessEngine::heuristicValueForState(State *state){
    if (state->_is_game_over){
        if (state->_is_draw)
            return 0;
        else
            return state->_white_won ? 100000 : -100000;
    }
    return simpleMaterialEvaluation(state);
}

float ChessEngine::simpleMaterialEvaluation(State *state){
    int whiteVal = 0;
    int blackVal = 0;

    for (int i = 0; i < state->_board.size(); i++){
        for (int j = 0; j < state->_board.at(i).size(); j++){
            Piece *piece = state->_board.at(i).at(j);
            if (piece != nullptr){
                if (piece->_type == Queen){
                    if (piece->_colour == White)
                        whiteVal += 9;
                    else
                        blackVal += 9;
                }
                else if (piece->_type == Queen){
                    if (piece->_colour == White)
                        whiteVal += 9;
                    else
                        blackVal += 9;
                }
                else if (piece->_type == Rook){
                    if (piece->_colour == White)
                        whiteVal += 5;
                    else
                        blackVal += 5;
                }
                else if (piece->_type == Bishop){
                    if (piece->_colour == White)
                        whiteVal += 3;
                    else
                        blackVal += 3;
                }
                else if (piece->_type == Knight){
                    if (piece->_colour == White)
                        whiteVal += 3;
                    else
                        blackVal += 3;
                }
                else if (piece->_type == Pawn){
                    if (piece->_colour == White)
                        whiteVal += 1;
                    else
                        blackVal += 1;
                }
            }
        }
    }

    return whiteVal - blackVal;
}

void ChessEngine::addAllPromotionSelections(State *state){
    vector<Move> promotionMoves;
    for (int i = 0; i < state->_legal_moves_from_state.size(); i++){
        Move move =  state->_legal_moves_from_state.at(i);
        if (move._move_type == Promotion || move._move_type == PromotionCapture){
            move._promotion_selection = Queen;
            Move knightPromotion = move;
            knightPromotion._promotion_selection = Knight;
            promotionMoves.push_back(knightPromotion);
            Move bishopPromotion = move;
            bishopPromotion._promotion_selection = Bishop;
            promotionMoves.push_back(bishopPromotion);
            Move rookPromotion = move;
            rookPromotion._promotion_selection = Rook;
            promotionMoves.push_back(rookPromotion);
        }
    }
    for (auto move: promotionMoves)
        state->_legal_moves_from_state.push_back(move);
}

Move ChessEngine::makeRandomMove(State *state){
    vector<Move> in = state->_legal_moves_from_state;
    vector<Move> out;
    sample(in.begin(),
           in.end(),
           back_inserter(out),
           1,
           mt19937{random_device{}()});
    return out[0];
}






