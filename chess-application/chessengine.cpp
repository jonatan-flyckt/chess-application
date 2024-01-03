#include "chessengine.h"

ChessEngine::ChessEngine(){
    _move_number_minimax_tree_map = new unordered_map<int, MiniMaxTree*>();
}

ChessEngine::~ChessEngine(){
    //TODO: delete all trees individually
    delete _move_number_minimax_tree_map;
}

Move ChessEngine::selectMoveFromState(State *state, Colour engineColour){
    cout << endl << "Game phase: " << state->_game_phase << endl;
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
    _rules._inner_accumulated_state_generation_time = 0;
    _rules._accumulated_get_legal_bit_board_moves_timer = 0;

    _rules._pawn_timer = 0;
    _rules._knight_timer = 0;
    _rules._bishop_timer = 0;
    _rules._rook_timer = 0;
    _rules._queen_timer = 0;
    _rules._king_timer = 0;
    _rules._castling_timer = 0;
    _rules._self_check_timer = 0;
    _rules._self_check_first_timer = 0;
    _rules._self_check_second_timer = 0;
    _rules._self_check_inner_timer = 0;

    _rules._attack_pawn_timer = 0;
    _rules._attack_knight_timer = 0;
    _rules._attack_bishop_timer = 0;
    _rules._attack_rook_timer = 0;


    uint64_t start = nanosecond_measurement();

    pair<Move, float> bestMoveEvalPair = alphaBeta(tree, startingNode, startingNode->_depth_of_node, INFINITY_NEG, INFINITY_POS, engineColour == White);

    qDebug() << "Total time for finding move:" << float(nanosecond_measurement() - start) / 1000000.0 << "ms";
    qDebug() << "Move generation time:" << float(_accumulated_move_generation_time) / 1000000.0 << "ms";
    qDebug() << "State generation time:" << float(_accumulated_state_generation_time) / 1000000.0 << "ms";
    qDebug() << "Inner state generation time:" << float(_rules._inner_accumulated_state_generation_time) / 1000000.0 << "ms";
    qDebug() << "Heuristic evaluation time:" << float(_accumulated_heuristic_evaluation_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated update bit board time:" << float(_rules._accumulated_update_bit_board_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated get legal bit board moves time:" << float(_rules._accumulated_get_legal_bit_board_moves_timer) / 1000000.0 << "ms";
    qDebug() << "Accumulated update castling time:" << float(_rules._accumulated_update_castling_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated kings in check time:" << float(_rules._accumulated_kings_in_check_time) / 1000000.0 << "ms";
    qDebug() << "Accumulated hash generation time:" << float(_rules._accumulated_hash_time) / 1000000.0 << "ms";
    qDebug() << "pseudo pawn time:" << float(_rules._pawn_timer) / 1000000.0 << "ms";
    qDebug() << "pseudo knight time:" << float(_rules._knight_timer) / 1000000.0 << "ms";
    qDebug() << "pseudo bishop time:" << float(_rules._bishop_timer) / 1000000.0 << "ms";
    qDebug() << "pseudo rook time:" << float(_rules._rook_timer) / 1000000.0 << "ms";
    qDebug() << "pseudo queen time:" << float(_rules._queen_timer) / 1000000.0 << "ms";
    qDebug() << "pseudo king time:" << float(_rules._king_timer) / 1000000.0 << "ms";
    qDebug() << "castling time:" << float(_rules._castling_timer) / 1000000.0 << "ms" << endl;
    qDebug() << "self check time:" << float(_rules._self_check_timer) / 1000000.0 << "ms";
    qDebug() << "self check inner time:" << float(_rules._self_check_inner_timer) / 1000000.0 << "ms";
    qDebug() << "self check first part:" << float(_rules._self_check_first_timer) / 1000000.0 << "ms";
    qDebug() << "self check second part:" << float(_rules._self_check_second_timer) / 1000000.0 << "ms";
    qDebug() << "attacking square pawn:" << float(_rules._attack_pawn_timer) / 1000000.0 << "ms";
    qDebug() << "attacking square knight:" << float(_rules._attack_knight_timer) / 1000000.0 << "ms";
    qDebug() << "attacking square bishop/queen:" << float(_rules._attack_bishop_timer) / 1000000.0 << "ms";
    qDebug() << "attacking square rook/queen:" << float(_rules._attack_rook_timer) / 1000000.0 << "ms";
    qDebug() << "";

    startingNode->_move_eval_pair.second = bestMoveEvalPair.second;
    tree->_best_move = bestMoveEvalPair.first;




    return tree->_best_move;
}

pair<Move, float> ChessEngine::alphaBeta(MiniMaxTree *tree, MiniMaxNode *node, int depth, float alpha, float beta, bool maximisingPlayer){

    if (depth == tree->_max_depth || node->_state->_is_game_over){
        uint64_t start = nanosecond_measurement();
        pair<Move, float> moveEvalPair;
        moveEvalPair.first = node->_state->_move_to_state;
        float value;
        if (node->_state->_is_game_over){
            if (node->_state->_is_draw){
                value = 0;
            }
            else{
                value =  node->_state->_white_won ? WHITE_WIN_EVAL: BLACK_WIN_EVAL;
            }
        }
        else{
            value = fastSimpleHeuristicValueForState(node->_state);
        }
        moveEvalPair.second = value;
        _accumulated_heuristic_evaluation_time += nanosecond_measurement() - start;
        return moveEvalPair;
    }
    uint64_t start = nanosecond_measurement();
    node->_state->_legal_moves_from_state = _rules.getLegalMoves(node->_state);
    _accumulated_move_generation_time += nanosecond_measurement() - start;

    //TODO: maybe keep this, maybe not
    std::random_shuffle ( node->_state->_legal_moves_from_state.begin(),
                          node->_state->_legal_moves_from_state.end() );

    start = nanosecond_measurement();

    for (auto legalMove: node->_state->_legal_moves_from_state){
        MiniMaxNode *newNode = new MiniMaxNode();
        newNode->_depth_of_node = depth+1;
        newNode->_state = _rules.getResultingStateFromMove(node->_state, legalMove);

        //without move ordering:
        //node->_children.push_back(newNode);

        //with move ordering:
        newNode->_heuristic_value = fastSimpleHeuristicValueForState(newNode->_state);
        insertSorted(node->_children, newNode, maximisingPlayer);
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

float ChessEngine::fastSimpleHeuristicValueForState(State *state){
    if (state->_is_game_over){
        if (state->_is_draw)
            return 0;
        else
            return state->_white_won ? WHITE_WIN_EVAL : BLACK_WIN_EVAL;
    }
    switch (state->_game_phase){
        case Opening:
            return simpleOpeningEvaluation(state);
        case MidGame:
            return simpleMidGameEvaluation(state);
        case EndGame:
            return simpleEndGameEvaluation(state);
        case LateEndGame:
            return simpleEndGameEvaluation(state);
        default:
            return simpleMidGameEvaluation(state);
    }

    //TODO: Create a solid function to determine state of game (opening, mid, end), rather than just using 30 moves
    //return state->_number_of_moves > 30 ? simpleMaterialEvaluation(state) :
    //                                      simpleOpeningEvaluation(state);
}

float ChessEngine::materialEvaluation(State *state){
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



float ChessEngine::simpleEndGameEvaluation(State *state){
    float eval = 0;
    float materialEval = materialEvaluation(state);
    eval += materialEval;
    eval += forceKingToEdgeInEndGame(state, materialEval);
    eval += encouragePawnPushing(state);
    return eval;
}

float ChessEngine::forceKingToEdgeInEndGame(State *state, float materialEval){
    //Very simple end-game evaluation to try to force opponent's king to the corner or edge of the board
    float opposingKingSmallCornerBonus = 0.2;
    float opposingKingLargeCornerBonus = 0.1;
    float opposingKingNonSmallCentreBonus = 0.05;
    float opposingKingNonMediumCentreBonus = 0.05;
    float opposingKingNonLargeCentreBonus = 0.05;
    int maximumMaterialEvalBonusLimit = 15;

    //The player who has the piece advantage should not be punished as much for having their king in the corner
    //That is, we are trying to help the player with a winning position to force the opponent's king to the edge
    //The player with advantage gets a bigger multiplier the bigger their material advantage is, up to 15, after which they always get a multiplier of 1
    float whiteWinningMultiplier = min(static_cast<float>(1), (max(materialEval / maximumMaterialEvalBonusLimit, static_cast<float>(0))));
    float blackWinningMultiplier = min(static_cast<float>(1), (max(materialEval / -maximumMaterialEvalBonusLimit, static_cast<float>(0))));

    float whiteVal = 0;
    float blackVal = 0;

    if (state->_bit_board._black_king & _small_square_centre_mask_complement)
        whiteVal += opposingKingNonSmallCentreBonus * whiteWinningMultiplier;
    if (state->_bit_board._black_king & _medium_square_centre_mask_complement)
        whiteVal += opposingKingNonMediumCentreBonus * whiteWinningMultiplier;
    if (state->_bit_board._black_king & _large_square_centre_mask_complement)
        whiteVal += opposingKingNonLargeCentreBonus * whiteWinningMultiplier;
    if (state->_bit_board._black_king & _large_corner_mask)
        whiteVal += opposingKingLargeCornerBonus * whiteWinningMultiplier;
    if (state->_bit_board._black_king & _small_corner_mask)
        whiteVal += opposingKingSmallCornerBonus * whiteWinningMultiplier;

    if (state->_bit_board._white_king & _small_square_centre_mask_complement)
        blackVal += opposingKingNonSmallCentreBonus * blackWinningMultiplier;
    if (state->_bit_board._white_king & _medium_square_centre_mask_complement)
        blackVal += opposingKingNonMediumCentreBonus * blackWinningMultiplier;
    if (state->_bit_board._white_king & _large_square_centre_mask_complement)
        blackVal += opposingKingNonLargeCentreBonus * blackWinningMultiplier;
    if (state->_bit_board._white_king & _large_corner_mask)
        blackVal += opposingKingLargeCornerBonus * blackWinningMultiplier;
    if (state->_bit_board._white_king & _small_corner_mask)
        blackVal += opposingKingSmallCornerBonus * blackWinningMultiplier;

    return materialEval + whiteVal - blackVal;
}

float ChessEngine::encouragePawnPushing(State *state, int multiplier){
    float seventhRankBonus = 0.8 * multiplier;
    float sixthRankBonus = 0.3 * multiplier;
    float fifthRankBonus = 0.1 * multiplier;
    float fourthRankBonus = 0.01 * multiplier;
    float thirdRankBonus = 0.01 * multiplier;

    float eval = 0;

    BitBoard board = state->_bit_board;
    float whitePawnsOnThird = (board._white_pawns & _third_rank_mask);
    float whitePawnsOnFourth = (board._white_pawns & _fourth_rank_mask);
    float whitePawnsOnFifth = (board._white_pawns & _fifth_rank_mask);
    float whitePawnsOnSixth = (board._white_pawns & _sixth_rank_mask);
    float whitePawnsOnSeventh = (board._white_pawns & _seventh_rank_mask);
    if (whitePawnsOnThird)
        eval += countBitsInBoard(whitePawnsOnThird) * thirdRankBonus;
    if (whitePawnsOnFourth)
        eval += countBitsInBoard(whitePawnsOnFourth) * fourthRankBonus;
    if (whitePawnsOnFifth)
        eval += countBitsInBoard(whitePawnsOnFifth) * fifthRankBonus;
    if (whitePawnsOnSixth)
        eval += countBitsInBoard(whitePawnsOnSixth) * sixthRankBonus;
    if (whitePawnsOnSeventh)
        eval += countBitsInBoard(whitePawnsOnSeventh) * seventhRankBonus;

    float blackPawnsOnThird = (board._black_pawns & _sixth_rank_mask);
    float blackPawnsOnFourth = (board._black_pawns & _fifth_rank_mask);
    float blackPawnsOnFifth = (board._black_pawns & _fourth_rank_mask);
    float blackPawnsOnSixth = (board._black_pawns & _third_rank_mask);
    float blackPawnsOnSeventh = (board._black_pawns & _second_rank_mask);
    if (blackPawnsOnThird)
        eval -= countBitsInBoard(blackPawnsOnThird) * thirdRankBonus;
    if (blackPawnsOnFourth)
        eval -= countBitsInBoard(blackPawnsOnFourth) * fourthRankBonus;
    if (blackPawnsOnFifth)
        eval -= countBitsInBoard(blackPawnsOnFifth) * fifthRankBonus;
    if (blackPawnsOnSixth)
        eval -= countBitsInBoard(blackPawnsOnSixth) * sixthRankBonus;
    if (blackPawnsOnSeventh)
        eval -= countBitsInBoard(blackPawnsOnSeventh) * seventhRankBonus;

    return eval;
}

float ChessEngine::simpleMidGameEvaluation(State *state){
    //Really encourage castling if not performed yet
    float castlingBonus = 1.5;
    float rooksOnSeventhBonus = 0.3;

    float whiteVal = 0;
    float blackVal = 0;

    //castling bonuses:
    if (state->_castling_info._white_castled)
        whiteVal += castlingBonus;
    if (state->_castling_info._black_castled)
        blackVal += castlingBonus;

    if (state->_bit_board._white_rooks & _seventh_rank_mask)
        whiteVal += rooksOnSeventhBonus;
    if (state->_bit_board._black_rooks & _second_rank_mask)
        blackVal += rooksOnSeventhBonus;

    return materialEvaluation(state) + encouragePawnPushing(state, 0.3) + whiteVal - blackVal;
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
    whiteVal += countBitsInBoard(state->_bit_board._white_knights & _medium_square_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_knights & _medium_rectangle_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_knights & _small_square_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_pawns & _medium_square_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_pawns & _medium_rectangle_centre_mask) * centreControlBonus;
    whiteVal += countBitsInBoard(state->_bit_board._white_pawns & _small_square_centre_mask) * centreControlBonus;

    blackVal += countBitsInBoard(state->_bit_board._black_knights & _medium_square_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_knights & _medium_rectangle_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_knights & _small_square_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_pawns & _medium_square_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_pawns & _medium_rectangle_centre_mask) * centreControlBonus;
    blackVal += countBitsInBoard(state->_bit_board._black_pawns & _small_square_centre_mask) * centreControlBonus;

    //castling bonuses:
    if (state->_castling_info._white_castled)
        whiteVal += castlingBonus;
    if (state->_castling_info._black_castled)
        blackVal += castlingBonus;

    return materialEvaluation(state) + whiteVal - blackVal;
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
// Function to insert a new node into the sorted vector
void ChessEngine::insertSorted(vector<MiniMaxNode*>& _children, MiniMaxNode* newNode, bool descending) {
    auto comp = MiniMaxNodeComparator(descending);
    auto it = std::lower_bound(_children.begin(), _children.end(), newNode, comp);
    _children.insert(it, newNode);
}
