#include "chessengine.h"

ChessEngine::ChessEngine(){
    _move_number_minimax_tree_map = new map<int, MiniMaxTree*>();
}

ChessEngine::~ChessEngine(){
    //TODO: delete all trees individually
    delete _move_number_minimax_tree_map;
}

Move ChessEngine::selectMoveFromState(State *state, Colour engineColour){
    return miniMax(state, engineColour);
    //return makeRandomMove(state);
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
    _accumulated_state_generation_time = 0;
    _accumulated_heuristic_evaluation_time = 0;
    _rules._accumulated_update_bit_board_time = 0;
    _rules._accumulated_update_castling_time = 0;
    _rules._accumulated_kings_in_check_time = 0;
    _rules._accumulated_hash_time = 0;
    uint64_t start = nanosecond_measurement();

    pair<Move, float> bestMoveEvalPair = alphaBeta(tree, startingNode, startingNode->_depth_of_node, INFINITY_NEG, INFINITY_POS, engineColour == White);

    qDebug() << "Total time for finding move:" << float(nanosecond_measurement() - start) / 1000000.0 << "ms";
    qDebug() << "Move generation time:" << float(_accumulated_move_generation_time) / 1000000.0 << "ms";
    qDebug() << "State generation time:" << float(_accumulated_state_generation_time) / 1000000.0 << "ms";
    qDebug() << "Heuristic evaluation time:" << float(_accumulated_heuristic_evaluation_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated update bit board time:" << float(_rules._accumulated_update_bit_board_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated update castling time:" << float(_rules._accumulated_update_castling_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated kings in check time:" << float(_rules._accumulated_kings_in_check_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated hash generation time:" << float(_rules._accumulated_hash_time) / 1000000.0 << "ms";
    qDebug() << "";

    startingNode->_move_eval_pair.second = bestMoveEvalPair.second;
    tree->_best_move = bestMoveEvalPair.first;




    return tree->_best_move;
}

pair<Move, float> ChessEngine::alphaBeta(MiniMaxTree *tree, MiniMaxNode *node, int depth, float alpha, float beta, bool maximisingPlayer){

    if (depth == tree->_max_depth || node->_state->_is_game_over){
        uint64_t start = nanosecond_measurement();
        float value = heuristicValueForState(node->_state);
        pair<Move, float> moveEvalPair;
        moveEvalPair.first = node->_state->_move_to_state;
        moveEvalPair.second = value;
        return moveEvalPair;
        _accumulated_heuristic_evaluation_time += nanosecond_measurement() - start;
    }
    uint64_t start = nanosecond_measurement();
    node->_state->_legal_moves_from_state = _rules.getLegalBitBoardMoves(node->_state);
    _accumulated_move_generation_time += nanosecond_measurement() - start;

    //TODO: maybe keep this, maybe not
    std::random_shuffle ( node->_state->_legal_moves_from_state.begin(),
                          node->_state->_legal_moves_from_state.end() );

    start = nanosecond_measurement();
    //TODO: move ordering
    for (auto legalMove: node->_state->_legal_moves_from_state){
        MiniMaxNode *newNode = new MiniMaxNode();
        newNode->_depth_of_node = depth+1;
        newNode->_state = _rules.getResultingStateFromMove(node->_state, legalMove);
        node->_children.push_back(newNode);
    }
    _accumulated_state_generation_time += nanosecond_measurement() - start;

    if (maximisingPlayer){ //Find best move for white (maximise)
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
            if (alpha >= beta)
                break;
        }
        return pair<Move, float>(bestMove, bestEval);
    }
    else{ //Find best move for black (minimise)
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
            if (beta <= alpha)
                break;
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
    //TODO: Create a solid function to determine state of game (opening, mid, end), rather than just using 30 moves
    return state->_number_of_moves > 30 ? simpleMaterialEvaluation(state) :
                                          simpleMaterialEvaluation(state) + simpleOpeningEvaluation(state);
}

float ChessEngine::simpleMaterialEvaluation(State *state){
    float whiteVal = 0;
    float blackVal = 0;
    whiteVal += countBitsInBoard(state->_bit_board._white_pawns) * 1;
    whiteVal += countBitsInBoard(state->_bit_board._white_knights) * 3;
    whiteVal += countBitsInBoard(state->_bit_board._white_bishops) * 3;
    whiteVal += countBitsInBoard(state->_bit_board._white_rooks) * 5;
    whiteVal += countBitsInBoard(state->_bit_board._white_queens) * 9;

    blackVal += countBitsInBoard(state->_bit_board._black_pawns) * 1;
    blackVal += countBitsInBoard(state->_bit_board._black_knights) * 3;
    blackVal += countBitsInBoard(state->_bit_board._black_bishops) * 3;
    blackVal += countBitsInBoard(state->_bit_board._black_rooks) * 5;
    blackVal += countBitsInBoard(state->_bit_board._black_queens) * 9;

    return whiteVal - blackVal;
}

float ChessEngine::simpleOpeningEvaluation(State *state){
    float startingSquarePenalty = 0.1;
    float centreControlBonus = 0.03;
    float kingAndRooksOnStartingSquareBonus = 0.15;
    float castlingBonus = 0.8;

    float whiteVal = 0;
    float blackVal = 0;

    //piece development bonuses:
    whiteVal -= countBitsInBoard(state->_bit_board._white_knights & _starting_bitboard[Piece(White, Knight)]) * startingSquarePenalty;
    whiteVal -= countBitsInBoard(state->_bit_board._white_bishops & _starting_bitboard[Piece(White, Bishop)]) * startingSquarePenalty;
    whiteVal -= countBitsInBoard(state->_bit_board._white_queens & _starting_bitboard[Piece(White, Queen)]) * startingSquarePenalty;
    whiteVal += countBitsInBoard(state->_bit_board._white_rooks & _starting_bitboard[Piece(White, Rook)]) * kingAndRooksOnStartingSquareBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_king & _starting_bitboard[Piece(White, King)]) * kingAndRooksOnStartingSquareBonus;

    blackVal -= countBitsInBoard(state->_bit_board._black_knights & _starting_bitboard[Piece(Black, Knight)]) * startingSquarePenalty;
    blackVal -= countBitsInBoard(state->_bit_board._black_bishops & _starting_bitboard[Piece(Black, Bishop)]) * startingSquarePenalty;
    blackVal -= countBitsInBoard(state->_bit_board._black_queens & _starting_bitboard[Piece(Black, Queen)]) * startingSquarePenalty;
    blackVal += countBitsInBoard(state->_bit_board._black_rooks & _starting_bitboard[Piece(Black, Rook)]) * kingAndRooksOnStartingSquareBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_king & _starting_bitboard[Piece(Black, King)]) * kingAndRooksOnStartingSquareBonus;

    //centre control bonuses:
    whiteVal += countBitsInBoard(state->_bit_board._white_knights & _large_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_knights & _medium_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_knights & _small_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_pawns & _large_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_pawns & _medium_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_pawns & _small_centre_mask) * centreControlBonus;

    blackVal += countBitsInBoard(state->_bit_board._black_knights & _large_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_knights & _medium_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_knights & _small_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_pawns & _large_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_pawns & _medium_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_pawns & _small_centre_mask) * centreControlBonus;

    //castling bonuses:
    if (state->_castling_info._white_castled)
        whiteVal += castlingBonus;
    if (state->_castling_info._black_castled)
        blackVal += castlingBonus;

    return whiteVal - blackVal;
}

int ChessEngine::countBitsInBoard(ULL board){
    int count;
    for (count = 0; board; count++, board &= board - 1);
    return count;
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






