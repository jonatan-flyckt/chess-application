#include "chessengine.h"

ChessEngine::ChessEngine(){
    _move_number_minimax_tree_map = new map<int, MiniMaxTree*>();
}

ChessEngine::~ChessEngine(){
    //TODO: delete all trees individually
    delete _move_number_minimax_tree_map;
}

Move ChessEngine::selectMoveFromState(State *state, Colour engineColour){
    addAllPromotionSelections(state);
    return miniMax(state, engineColour);
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

    _accumulated_move_generation_time = 0;
    _accumulated_heuristic_evaluation_time = 0;
    _accumulated_alpha_beta_time = 0;
    _rules._accumulated_test_time = 0;


    _perft_one = 0;
    _perft_two = 0;
    _perft_three = 0;
    _perft_four = 0;

    pair<Move, float> bestMoveEvalPair = alphaBeta(tree, startingNode, startingNode->_depth_of_node, INFINITY_NEG, INFINITY_POS, engineColour == White);

    qDebug() << "Move generation time:" << _accumulated_move_generation_time;
    qDebug() << "Heuristic evaluation time:" << _accumulated_heuristic_evaluation_time;
    qDebug() << "Alpha-Beta time:" << _accumulated_alpha_beta_time;
    qDebug() << "Test timer:" << _rules._accumulated_test_time;
    qDebug() << "PERFT: " << _perft_one << " " << _perft_two << " " << _perft_three << " " << _perft_four;

    startingNode->_move_eval_pair.second = bestMoveEvalPair.second;
    tree->_best_move = bestMoveEvalPair.first;
    return tree->_best_move;
}

pair<Move, float> ChessEngine::alphaBeta(MiniMaxTree *tree, MiniMaxNode *node, int depth, float alpha, float beta, bool maximisingPlayer){

    if (depth == tree->_max_depth || node->_state->_is_game_over){
        _heuristic_evaluation_timer.start();
        float value = heuristicValueForState(node->_state);
        pair<Move, float> moveEvalPair;
        moveEvalPair.first = node->_state->_move_to_state;
        moveEvalPair.second = value;
        return moveEvalPair;
        _accumulated_heuristic_evaluation_time += _heuristic_evaluation_timer.elapsed();
    }
    _move_generation_timer.start();
    node->_state->_legal_moves_from_state = _rules.getLegalMoves(node->_state);
    if (depth == 0)
        _perft_one += node->_state->_legal_moves_from_state.size();
    else if (depth == 1)
        _perft_two += node->_state->_legal_moves_from_state.size();
    else if (depth == 2)
        _perft_three += node->_state->_legal_moves_from_state.size();
    else if (depth == 3)
        _perft_four += node->_state->_legal_moves_from_state.size();

    //TODO: move ordering
    for (auto legalMove: node->_state->_legal_moves_from_state){
        MiniMaxNode *newNode = new MiniMaxNode();
        newNode->_depth_of_node = depth+1;
        newNode->_state = _rules.getResultingStateFromMove(node->_state, legalMove);
        node->_children.push_back(newNode);
    }
    _accumulated_move_generation_time += _move_generation_timer.elapsed();


    if (maximisingPlayer){ //Find best move for white (maximise)
        _alpha_beta_timer.start();
        float bestEval = INFINITY_NEG;
        Move bestMove;
        for (auto child: node->_children){
            pair<Move, float> moveEval = alphaBeta(tree, child, depth+1, alpha, beta, false);
            child->_move_eval_pair = moveEval;
            if (moveEval.second > bestEval){
                bestMove = child->_state->_move_to_state;
                bestEval = moveEval.second;
            }
            alpha = max(alpha, bestEval);
            //Prune tree if possible:
            //if (alpha >= beta)
            //    break;
        }
        _accumulated_alpha_beta_time += _alpha_beta_timer.elapsed();
        return pair<Move, float>(bestMove, bestEval);
    }
    else{ //Find best move for black (minimise)
        _alpha_beta_timer.start();
        float bestEval = INFINITY_POS;
        Move bestMove;
        for (auto child: node->_children){
            pair<Move, float> moveEval = alphaBeta(tree, child, depth+1, alpha, beta, true);
            child->_move_eval_pair = moveEval;
            if (moveEval.second < bestEval){
                bestMove = child->_state->_move_to_state;
                bestEval = moveEval.second;
            }
            beta = min(beta, bestEval);
            //Prune tree if possible:
            //if (beta <= alpha)
            //    break;
        }
        _accumulated_alpha_beta_time += _alpha_beta_timer.elapsed();
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






