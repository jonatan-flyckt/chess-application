#include "chessrules.h"

ChessRules::ChessRules(){
}

uint64_t nanosecond_measurement()
{
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return ns;
}

State* ChessRules::getResultingStateFromMove(State *currentState, Move moveToMake){
    uint64_t whole_func_start = nanosecond_measurement();

    State *resultingState = new State();
    uint64_t start = nanosecond_measurement();

    updateBitBoardWithMove(currentState, resultingState, moveToMake);
    resultingState->_indices_of_bits_for_piece_types = getIndicesOfBitsForPieceTypes(resultingState->_bit_board);
    _accumulated_update_bit_board_time += nanosecond_measurement() - start;

    resultingState->_bit_board_state_seen_count = currentState->_bit_board_state_seen_count;
    resultingState->_colour_to_move = currentState->_colour_to_move == White ? Black : White;
    resultingState->_move_to_state = moveToMake;
    resultingState->_previous_state = currentState;
    resultingState->_number_of_moves = currentState->_number_of_moves+1;
    resultingState->_moves_without_capture_or_pawn_advancement = (moveToMake._move_type == Capture || moveToMake._piece._type == Pawn) ? 0 : currentState->_moves_without_capture_or_pawn_advancement+1;
    resultingState->_castling_info = currentState->_castling_info;

    start = nanosecond_measurement();
    updateCastlingInfo(moveToMake, resultingState);
    _accumulated_update_castling_time += nanosecond_measurement() - start;

    start = nanosecond_measurement();
    resultingState->_legal_moves_from_state = getLegalBitBoardMoves(resultingState);
    _accumulated_get_legal_bit_board_moves_timer += nanosecond_measurement() - start;

    start = nanosecond_measurement();
    resultingState->_white_king_is_in_check = bitBoardWhiteKingIsInCheck(resultingState->_bit_board);
    resultingState->_black_king_is_in_check = bitBoardBlackKingIsInCheck(resultingState->_bit_board);
    _accumulated_kings_in_check_time += nanosecond_measurement() - start;


    if (resultingState->_legal_moves_from_state.size() == 0){ //End the game if there are no legal moves
        resultingState->_is_game_over = true;

        if (resultingState->_colour_to_move == Black && resultingState->_black_king_is_in_check){
            resultingState->_white_won = true;
            resultingState->_game_over_reason = "Check Mate";
        }
        else if (resultingState->_colour_to_move == White && resultingState->_white_king_is_in_check){
            resultingState->_black_won = true;
            resultingState->_game_over_reason = "Check Mate";
        }
        else{
            resultingState->_is_draw = true;
            resultingState->_game_over_reason = "Stalemate";
        }
    }
    //if (isInsufficientMaterial(currentState)){ //TODO: Replace
    //    currentState->_is_game_over = true;
    //    resultingState->_is_draw = true;
    //    resultingState->_game_over_reason = "Insufficient mating material";
    //}
    if (resultingState->_moves_without_capture_or_pawn_advancement >= 100){
        resultingState->_is_game_over = true;
        resultingState->_is_draw = true;
        resultingState->_game_over_reason = "50 move rule";
    }

    start = nanosecond_measurement();
    resultingState->_position_hash = _hasher.generateHashForPosition(resultingState->_indices_of_bits_for_piece_types,
                                                                     resultingState->_castling_info,
                                                                     resultingState->_colour_to_move,
                                                                     getIndicesOfBitsInBoard(resultingState->_bit_board._en_passant_square));
    _accumulated_hash_time += nanosecond_measurement() - start;


    //TODO: incorporate three move repetition in minimax search as well


    _inner_accumulated_state_generation_time += nanosecond_measurement() - whole_func_start;

    return resultingState;
}

void ChessRules::updateCastlingInfo(Move move, State *state){

    if (move._destination_square == "a1")
        state->_castling_info._white_long_rook_has_moved = true;
    else if (move._destination_square == "h1")
        state->_castling_info._white_short_rook_has_moved = true;
    else if (move._destination_square == "a8")
        state->_castling_info._black_long_rook_has_moved = true;
    else if (move._destination_square == "h8")
        state->_castling_info._black_short_rook_has_moved = true;

    if (move._piece._type == Rook){
        if (move._piece._colour == White){
            if (move._origin_square == "a1")
                state->_castling_info._white_long_rook_has_moved = true;
            else if (move._origin_square == "h1")
                state->_castling_info._white_short_rook_has_moved = true;
        }
        else{
            if (move._origin_square == "a8")
                state->_castling_info._black_long_rook_has_moved = true;
            else if (move._origin_square == "h8")
                state->_castling_info._black_short_rook_has_moved = true;
        }
    }
    else if (move._piece._type == King){
        if (move._piece._colour == White)
            state->_castling_info._white_king_has_moved = true;
        else
            state->_castling_info._black_king_has_moved = true;
    }
    if (move._move_type == LongCastle || move._move_type == ShortCastle){
        if (move._piece._colour == White){
            state->_castling_info._white_castled = true;
            state->_castling_info._white_king_has_moved = true;
        }
        else{
            state->_castling_info._black_castled = true;
            state->_castling_info._black_king_has_moved = true;
        }
    }
}

vector<Move> ChessRules::getLegalBitBoardMoves(State *state){
    if (state->_move_to_state._piece._type == Pawn){ //Check if previous allowed for en passant
        if (abs(_index_from_square[state->_move_to_state._origin_square] - _index_from_square[state->_move_to_state._destination_square]) == 16){
            state->_bit_board._en_passant_square = _bit_masks[state->_colour_to_move == White ?
                        _index_from_square[state->_move_to_state._destination_square] + 8 :
                    _index_from_square[state->_move_to_state._destination_square] - 8];
        }
        else
            state->_bit_board._en_passant_square = 0;
    }
    else
        state->_bit_board._en_passant_square = 0;


    vector<int> pawnIndices = getIndicesOfBitsInBoard(state->_colour_to_move == White ? state->_bit_board._white_pawns : state->_bit_board._black_pawns);
    vector<Move> pawnMoves;
    for (auto index: pawnIndices)
        for (auto move: getBitBoardPseudoMovesForPawn(index, state->_bit_board, state->_colour_to_move, state->_number_of_moves, state->_bit_board._en_passant_square))
            pawnMoves.push_back(move);

    vector<int> knightIndices = getIndicesOfBitsInBoard(state->_colour_to_move == White ? state->_bit_board._white_knights : state->_bit_board._black_knights);
    vector<Move> knightMoves;
    for (auto index: knightIndices)
        for (auto move: getBitBoardPseudoMovesForKnight(index, state->_bit_board, state->_colour_to_move, state->_number_of_moves))
            knightMoves.push_back(move);

    int kingIndex = getIndicesOfBitsInBoard(state->_colour_to_move == White ? state->_bit_board._white_king : state->_bit_board._black_king)[0];
    vector<Move> kingMoves;
    for (auto move: getBitBoardPseudoMovesForKing(kingIndex, state->_bit_board, state->_colour_to_move, state->_number_of_moves))
        kingMoves.push_back(move);

    vector<int> bishopIndices = getIndicesOfBitsInBoard(state->_colour_to_move == White ? state->_bit_board._white_bishops : state->_bit_board._black_bishops);
    vector<Move> bishopMoves;
    for (auto index: bishopIndices)
        for (auto move: getBitBoardPseudoMovesForBishop(index, state->_bit_board, state->_colour_to_move, state->_number_of_moves))
            bishopMoves.push_back(move);

    vector<int> rookIndices = getIndicesOfBitsInBoard(state->_colour_to_move == White ? state->_bit_board._white_rooks : state->_bit_board._black_rooks);
    vector<Move> rookMoves;
    for (auto index: rookIndices)
        for (auto move: getBitBoardPseudoMovesForRook(index, state->_bit_board, state->_colour_to_move, state->_number_of_moves))
            rookMoves.push_back(move);

    vector<int> queenIndices = getIndicesOfBitsInBoard(state->_colour_to_move == White ? state->_bit_board._white_queens : state->_bit_board._black_queens);
    vector<Move> queenMoves;
    for (auto index: queenIndices)
        for (auto move: getBitBoardPseudoMovesForQueen(index, state->_bit_board, state->_colour_to_move, state->_number_of_moves))
            queenMoves.push_back(move);

    vector<Move> castlingMoves = getBitBoardCastlingMoves(state->_bit_board, state->_castling_info, state->_colour_to_move, state->_number_of_moves);

    vector<Move> allMoves;
    allMoves.reserve( knightMoves.size() + pawnMoves.size() + kingMoves.size() + bishopMoves.size() + rookMoves.size() + queenMoves.size() + castlingMoves.size());
    allMoves.insert( allMoves.end(), knightMoves.begin(), knightMoves.end() );
    allMoves.insert( allMoves.end(), pawnMoves.begin(), pawnMoves.end() );
    allMoves.insert( allMoves.end(), kingMoves.begin(), kingMoves.end() );
    allMoves.insert( allMoves.end(), bishopMoves.begin(), bishopMoves.end() );
    allMoves.insert( allMoves.end(), rookMoves.begin(), rookMoves.end() );
    allMoves.insert( allMoves.end(), queenMoves.begin(), queenMoves.end() );
    allMoves.insert( allMoves.end(), castlingMoves.begin(), castlingMoves.end() );

    //Remove all moves that caused self check:
    vector<Move>::iterator iter = allMoves.begin();
    while(iter != allMoves.end())
        bitBoardMoveCausedSelfCheck(*iter, state->_bit_board) ? iter = allMoves.erase(iter) : iter++;

    return allMoves;
}

bool ChessRules::bitBoardMoveCausedSelfCheck(Move move, BitBoard board){
    ULL destinationMask = _bit_masks[_index_from_square[move._destination_square]];
    ULL destinationComplementMask = _bit_masks_complement[_index_from_square[move._destination_square]];
    ULL originComplementMask = _bit_masks_complement[_index_from_square[move._origin_square]];

    if (move._colour_performing_move == White){
        if (move._piece._type == Pawn)
            board._white_pawns = (board._white_pawns | destinationMask) & originComplementMask;
        if (move._piece._type == Knight)
            board._white_knights = (board._white_knights | destinationMask) & originComplementMask;
        if (move._piece._type == Bishop)
            board._white_bishops = (board._white_bishops | destinationMask) & originComplementMask;
        if (move._piece._type == Rook)
            board._white_rooks = (board._white_rooks | destinationMask) & originComplementMask;
        if (move._piece._type == Queen)
            board._white_queens = (board._white_queens | destinationMask) & originComplementMask;
        if (move._piece._type == King)
            board._white_king = (board._white_king | destinationMask) & originComplementMask;

        if (move._move_type == EnPassant){
            board._black_pawns &= ~(1ULL<<(getIndexOfLeastSignificantBit(board._en_passant_square)-8));
        }
        else if (move._move_type == LongCastle){
            board._white_king = _bit_masks[_index_from_square["c1"]];
            board._white_rooks =
                    (board._white_rooks & _bit_masks_complement[_index_from_square["a1"]]) | _bit_masks[_index_from_square["d1"]];
        }
        else if (move._move_type == ShortCastle){
            board._white_king = _bit_masks[_index_from_square["g1"]];
            board._white_rooks =
                    (board._white_rooks & _bit_masks_complement[_index_from_square["h1"]]) | _bit_masks[_index_from_square["f1"]];
        }
        else if (move._move_type == Promotion || move._move_type == PromotionCapture){
            board._white_pawns &= (destinationComplementMask ^ _bit_masks[_index_from_square[move._origin_square]]);
            if (move._promotion_selection == Queen)
                board._white_queens |= destinationMask;
            else if (move._promotion_selection == Knight)
                board._white_knights |= destinationMask;
            else if (move._promotion_selection == Bishop)
                board._white_bishops |= destinationMask;
            else if(move._promotion_selection == Rook)
                board._white_rooks |= destinationMask;
        }

        board._black_pawns &= destinationComplementMask;
        board._black_rooks &= destinationComplementMask;
        board._black_knights &= destinationComplementMask;
        board._black_bishops &= destinationComplementMask;
        board._black_queens &= destinationComplementMask;
    }
    else{
        if (move._piece._type == Pawn)
            board._black_pawns = (board._black_pawns | destinationMask) & originComplementMask;
        if (move._piece._type == Knight)
            board._black_knights = (board._black_knights | destinationMask) & originComplementMask;
        if (move._piece._type == Bishop)
            board._black_bishops = (board._black_bishops | destinationMask) & originComplementMask;
        if (move._piece._type == Rook)
            board._black_rooks = (board._black_rooks | destinationMask) & originComplementMask;
        if (move._piece._type == Queen)
            board._black_queens = (board._black_queens | destinationMask) & originComplementMask;
        if (move._piece._type == King)
            board._black_king = (board._black_king | destinationMask) & originComplementMask;

        if (move._move_type == EnPassant){
            board._white_pawns &= ~(1ULL<<(getIndexOfLeastSignificantBit(board._en_passant_square)+8));
        }
        else if (move._move_type == LongCastle){
            board._black_king = _bit_masks[_index_from_square["c8"]];
            board._black_rooks =
                    (board._black_rooks & _bit_masks_complement[_index_from_square["a8"]]) | _bit_masks[_index_from_square["d8"]];
        }
        else if (move._move_type == ShortCastle){
            board._black_king = _bit_masks[_index_from_square["g8"]];
            board._black_rooks =
                    (board._black_rooks & _bit_masks_complement[_index_from_square["h8"]]) | _bit_masks[_index_from_square["f8"]];
        }
        else if (move._move_type == Promotion || move._move_type == PromotionCapture){
            board._black_pawns &= (destinationComplementMask ^ _bit_masks[_index_from_square[move._origin_square]]);
            if (move._promotion_selection == Queen)
                board._black_queens |= destinationMask;
            else if (move._promotion_selection == Knight)
                board._black_knights |= destinationMask;
            else if (move._promotion_selection == Bishop)
                board._black_bishops |= destinationMask;
            else if(move._promotion_selection == Rook)
                board._black_rooks |= destinationMask;
        }

        board._white_pawns &= destinationComplementMask;
        board._white_rooks &= destinationComplementMask;
        board._white_knights &= destinationComplementMask;
        board._white_bishops &= destinationComplementMask;
        board._white_queens &= destinationComplementMask;
    }
    board._all_black_pieces = board._black_pawns | board._black_rooks | board._black_knights | board._black_bishops | board._black_queens | board._black_king;
    board._all_white_pieces = board._white_pawns | board._white_rooks | board._white_knights | board._white_bishops | board._white_queens | board._white_king;
    board._all_pieces = board._all_white_pieces | board._all_black_pieces;

    return move._colour_performing_move == White ? bitBoardWhiteKingIsInCheck(board): bitBoardBlackKingIsInCheck(board);
}

bool ChessRules::bitBoardWhiteKingIsInCheck(BitBoard board){
    return bitBoardSquareIsUnderAttack(getIndicesOfBitsInBoard(board._white_king).at(0), board, Black);
}

bool ChessRules::bitBoardBlackKingIsInCheck(BitBoard board){
    return bitBoardSquareIsUnderAttack(getIndicesOfBitsInBoard(board._black_king).at(0), board, White);
}

void ChessRules::updateBitBoardWithMove(State *currentState, State *resultingState, Move move){
    resultingState->_bit_board = currentState->_bit_board;
    int originIndex = _index_from_square[move._origin_square];
    int destinationIndex = _index_from_square[move._destination_square];
    ULL originComplement = _bit_masks_complement[originIndex];
    ULL destinationMask = _bit_masks[destinationIndex];
    ULL destinationComplement = _bit_masks_complement[destinationIndex];
    if (move._piece._colour == White){
        if (move._move_type == EnPassant){
            resultingState->_bit_board._black_pawns &= ~(1ULL<<(getIndexOfLeastSignificantBit(resultingState->_bit_board._en_passant_square)-8));
            resultingState->_bit_board._white_pawns = (currentState->_bit_board._white_pawns & originComplement) | destinationMask;
        }
        else if (move._move_type == LongCastle){
            resultingState->_bit_board._white_king = _bit_masks[_index_from_square["c1"]];
            resultingState->_bit_board._white_rooks =
                    (currentState->_bit_board._white_rooks & _bit_masks_complement[_index_from_square["a1"]]) | _bit_masks[_index_from_square["d1"]];
        }
        else if (move._move_type == ShortCastle){
            resultingState->_bit_board._white_king = _bit_masks[_index_from_square["g1"]];
            resultingState->_bit_board._white_rooks =
                    (currentState->_bit_board._white_rooks & _bit_masks_complement[_index_from_square["h1"]]) | _bit_masks[_index_from_square["f1"]];
        }
        else if (move._move_type == Promotion || move._move_type == PromotionCapture){
            resultingState->_bit_board._white_pawns &= (destinationComplement ^ _bit_masks[originIndex]);
            if (move._promotion_selection == Queen)
                resultingState->_bit_board._white_queens |= destinationMask;
            else if (move._promotion_selection == Knight)
                resultingState->_bit_board._white_knights |= destinationMask;
            else if (move._promotion_selection == Bishop)
                resultingState->_bit_board._white_bishops |= destinationMask;
            else if(move._promotion_selection == Rook)
                    resultingState->_bit_board._white_rooks |= destinationMask;
        }
        else if (move._piece._type == Pawn)
            resultingState->_bit_board._white_pawns = (currentState->_bit_board._white_pawns & originComplement) | destinationMask;
        else if (move._piece._type == Knight)
            resultingState->_bit_board._white_knights = (currentState->_bit_board._white_knights & originComplement) | destinationMask;
        else if (move._piece._type == Bishop)
            resultingState->_bit_board._white_bishops = (currentState->_bit_board._white_bishops & originComplement) | destinationMask;
        else if (move._piece._type == Rook)
            resultingState->_bit_board._white_rooks = (currentState->_bit_board._white_rooks & originComplement) | destinationMask;
        else if (move._piece._type == Queen)
            resultingState->_bit_board._white_queens = (currentState->_bit_board._white_queens & originComplement) | destinationMask;
        else
            resultingState->_bit_board._white_king = (currentState->_bit_board._white_king & originComplement) | destinationMask;
        resultingState->_bit_board._black_bishops &= destinationComplement;
        resultingState->_bit_board._black_knights &= destinationComplement;
        resultingState->_bit_board._black_rooks &= destinationComplement;
        resultingState->_bit_board._black_pawns &= destinationComplement;
        resultingState->_bit_board._black_queens &= destinationComplement;

    }
    else{
        if (move._move_type == EnPassant){
            resultingState->_bit_board._white_pawns &= ~(1ULL<<(getIndexOfLeastSignificantBit(resultingState->_bit_board._en_passant_square)+8));
            resultingState->_bit_board._black_pawns = (currentState->_bit_board._black_pawns & originComplement) | destinationMask;
        }
        else if (move._move_type == LongCastle){
            resultingState->_bit_board._black_king = _bit_masks[_index_from_square["c8"]];
            resultingState->_bit_board._black_rooks =
                    (currentState->_bit_board._black_rooks & _bit_masks_complement[_index_from_square["a8"]]) | _bit_masks[_index_from_square["d8"]];
        }
        else if (move._move_type == ShortCastle){
            resultingState->_bit_board._black_king = _bit_masks[_index_from_square["g8"]];
            resultingState->_bit_board._black_rooks =
                    (currentState->_bit_board._black_rooks & _bit_masks_complement[_index_from_square["h8"]]) | _bit_masks[_index_from_square["f8"]];
        }
        else if (move._move_type == Promotion || move._move_type == PromotionCapture){
            resultingState->_bit_board._black_pawns &= (destinationComplement ^ _bit_masks[originIndex]);
            if (move._promotion_selection == Queen)
                resultingState->_bit_board._black_queens |= destinationMask;
            else if (move._promotion_selection == Knight)
                resultingState->_bit_board._black_knights |= destinationMask;
            else if (move._promotion_selection == Bishop)
                resultingState->_bit_board._black_bishops |= destinationMask;
            else if(move._promotion_selection == Rook)
                    resultingState->_bit_board._black_rooks |= destinationMask;
        }
        else if (move._piece._type == Pawn)
            resultingState->_bit_board._black_pawns = (currentState->_bit_board._black_pawns & originComplement) | destinationMask;
        else if (move._piece._type == Knight)
            resultingState->_bit_board._black_knights = (currentState->_bit_board._black_knights & originComplement) | destinationMask;
        else if (move._piece._type == Bishop)
            resultingState->_bit_board._black_bishops = (currentState->_bit_board._black_bishops & originComplement) | destinationMask;
        else if (move._piece._type == Rook)
            resultingState->_bit_board._black_rooks = (currentState->_bit_board._black_rooks & originComplement) | destinationMask;
        else if (move._piece._type == Queen)
            resultingState->_bit_board._black_queens = (currentState->_bit_board._black_queens & originComplement) | destinationMask;
        else
            resultingState->_bit_board._black_king = (currentState->_bit_board._black_king & originComplement) | destinationMask;
        resultingState->_bit_board._white_bishops &= destinationComplement;
        resultingState->_bit_board._white_knights &= destinationComplement;
        resultingState->_bit_board._white_rooks &= destinationComplement;
        resultingState->_bit_board._white_pawns &= destinationComplement;
        resultingState->_bit_board._white_queens &= destinationComplement;
    }

    resultingState->_bit_board._all_white_pieces = resultingState->_bit_board._white_king | resultingState->_bit_board._white_pawns |
            resultingState->_bit_board._white_rooks | resultingState->_bit_board._white_bishops |
            resultingState->_bit_board._white_knights | resultingState->_bit_board._white_queens;
    resultingState->_bit_board._all_black_pieces = resultingState->_bit_board._black_king | resultingState->_bit_board._black_pawns |
            resultingState->_bit_board._black_rooks | resultingState->_bit_board._black_bishops |
            resultingState->_bit_board._black_knights | resultingState->_bit_board._black_queens;
    resultingState->_bit_board._all_pieces = resultingState->_bit_board._all_white_pieces | resultingState->_bit_board._all_black_pieces;
}

bool ChessRules::bitBoardSquareIsUnderAttack(int index, BitBoard board, Colour colourAttacking){
    ULL superPiece = _bit_masks[index];

    vector<int> attackingPawns = getIndicesOfBitsInBoard(colourAttacking == White ? board._white_pawns : board._black_pawns);
    vector<int> attackingKnights = getIndicesOfBitsInBoard(colourAttacking == White ? board._white_knights : board._black_knights);
    vector<int> attackingBishops = getIndicesOfBitsInBoard(colourAttacking == White ? board._white_bishops : board._black_bishops);
    vector<int> attackingRooks = getIndicesOfBitsInBoard(colourAttacking == White ? board._white_rooks : board._black_rooks);
    vector<int> attackingQueens = getIndicesOfBitsInBoard(colourAttacking == White ? board._white_queens : board._black_queens);
    int attackingKing = getIndicesOfBitsInBoard(colourAttacking == White ? board._white_king : board._black_king).at(0);

    for (auto piece: attackingQueens){
        ULL moves = getBitBoardOfPossibleAttacksForBishop(piece, board._all_pieces);
        moves |= getBitBoardOfPossibleAttacksForRook(piece, board._all_pieces);
        if (moves & superPiece) //The possible moves intersected with the square
            return true;
    }
    for (auto piece: attackingBishops){
        ULL moves = getBitBoardOfPossibleAttacksForBishop(piece, board._all_pieces);
        if (moves & superPiece) //The possible moves intersected with the square
            return true;
    }
    for (auto piece: attackingRooks){
        ULL moves = getBitBoardOfPossibleAttacksForRook(piece, board._all_pieces);
        if (moves & superPiece) //The possible moves intersected with the square
            return true;
    }
    for (auto piece: attackingKnights){
        ULL moves = _knight_move_set[piece];
        if (moves & superPiece) //The possible moves intersected with the square
            return true;
    }
    for (auto piece: attackingPawns){
        ULL moves = colourAttacking == White ? _white_pawn_capture_set[piece] : _black_pawn_capture_set[piece];
        if (moves & superPiece) //The possible moves intersected with the square
            return true;
    }
    if (superPiece & _king_move_set[attackingKing]) //The possible moves intersected with the square
        return true;

    return false;
}

vector<Move> ChessRules::getBitBoardPseudoMovesForRook(int index, BitBoard board, Colour colourToMove, int numberOfMoves){
    ULL possibleAttacks = getBitBoardOfPossibleAttacksForRook(index, board._all_pieces);
    ULL pseudoLegalMoves = colourToMove == White ? possibleAttacks &~board._all_white_pieces : possibleAttacks &~board._all_black_pieces;

    vector<Move> moveVector;

    for (auto resultingIndex : getIndicesOfBitsInBoard(pseudoLegalMoves)){
        MoveType moveType = _bit_masks[resultingIndex] & (colourToMove == White ? board._all_black_pieces : board._all_white_pieces) ? Capture : Standard;
        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Rook), _square_from_index[index],_square_from_index[resultingIndex],
                                  numberOfMoves+1, moveType));
    }
    return moveVector;
}

vector<Move> ChessRules::getBitBoardPseudoMovesForQueen(int index, BitBoard board, Colour colourToMove, int numberOfMoves){
    vector<Move> rookMoves = getBitBoardPseudoMovesForRook(index, board, colourToMove, numberOfMoves);
    vector<Move> bishopMoves = getBitBoardPseudoMovesForBishop(index, board, colourToMove, numberOfMoves);
    vector<Move> queenMoves;
    queenMoves.reserve( rookMoves.size() + bishopMoves.size() );
    queenMoves.insert( queenMoves.end(), rookMoves.begin(), rookMoves.end() );
    queenMoves.insert( queenMoves.end(), bishopMoves.begin(), bishopMoves.end() );
    for (int i = 0; i < queenMoves.size(); i++)
        queenMoves[i]._piece._type = Queen;
    return queenMoves;
}

vector<Move> ChessRules::getBitBoardPseudoMovesForKnight(int index, BitBoard board, Colour colourToMove, int numberOfMoves){
    ULL possibleAttacks = _knight_move_set[index];
    ULL pseudoLegalMoves = colourToMove == White ? possibleAttacks &~board._all_white_pieces : possibleAttacks &~board._all_black_pieces;

    vector<Move> moveVector;

    for (auto resultingIndex : getIndicesOfBitsInBoard(pseudoLegalMoves)){
        MoveType moveType = _bit_masks[resultingIndex] & (colourToMove == White ? board._all_black_pieces : board._all_white_pieces) ? Capture : Standard;
        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Knight), _square_from_index[index],_square_from_index[resultingIndex],
                                  numberOfMoves+1, moveType));
    }
    return moveVector;
}

vector<Move> ChessRules::getBitBoardPseudoMovesForBishop(int index, BitBoard board, Colour colourToMove, int numberOfMoves){
    ULL possibleAttacks = getBitBoardOfPossibleAttacksForBishop(index, board._all_pieces);
    ULL pseudoLegalMoves = colourToMove == White ? possibleAttacks &~board._all_white_pieces : possibleAttacks &~board._all_black_pieces;

    vector<Move> moveVector;

    for (auto resultingIndex : getIndicesOfBitsInBoard(pseudoLegalMoves)){
        MoveType moveType = _bit_masks[resultingIndex] & (colourToMove == White ? board._all_black_pieces : board._all_white_pieces) ? Capture : Standard;
        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Bishop), _square_from_index[index],_square_from_index[resultingIndex],
                                  numberOfMoves+1, moveType));
    }
    return moveVector;
}

vector<Move> ChessRules::getBitBoardPseudoMovesForPawn(int index, BitBoard board, Colour colourToMove, int numberOfMoves, ULL enPassantSquare){
    ULL possiblePushes = colourToMove == White ? _white_pawn_move_set[index] : _black_pawn_move_set[index];
    ULL possibleCaptures = colourToMove == White ? _white_pawn_capture_set[index] : _black_pawn_capture_set[index]; //TODO: check if this is an error, should be capture_set?
    ULL pseudoLegalPushes = possiblePushes &~board._all_pieces;
    ULL pseudoLegalCaptures = colourToMove == White ? possibleCaptures &board._all_black_pieces : possibleCaptures &board._all_white_pieces;

    if ((index > 7 && index < 16 && colourToMove == White) || (index < 56 && index > 47 && colourToMove == Black)){//Pawn was on starting square
        if (colourToMove == White)
            if (!((_bit_masks[index] << 8) &pseudoLegalPushes)) //Remove potential 2 step move if first square was blocked
                pseudoLegalPushes = 0;
        if (colourToMove == Black)
            if (!((_bit_masks[index] >> 8) &pseudoLegalPushes)) //Remove potential 2 step move if first square was blocked
                pseudoLegalPushes = 0;
    }

    vector<Move> moveVector;
    for (auto resultingIndex : getIndicesOfBitsInBoard(pseudoLegalPushes | pseudoLegalCaptures)){
        MoveType typeOfMove;
        uint32_t absVal = abs(index - resultingIndex);
        if ((index > 47 && colourToMove == White) || (index < 16 && colourToMove == Black)){ //Promotion move
            typeOfMove = (absVal == 8 || absVal == 16) ? Promotion : PromotionCapture;
            moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],_square_from_index[resultingIndex],
                                      numberOfMoves+1, typeOfMove, Queen));
            moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],_square_from_index[resultingIndex],
                                      numberOfMoves+1, typeOfMove, Rook));
            moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],_square_from_index[resultingIndex],
                                      numberOfMoves+1, typeOfMove, Knight));
            moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],_square_from_index[resultingIndex],
                                      numberOfMoves+1, typeOfMove, Bishop));
        }
        else{
            typeOfMove = (absVal == 8 || absVal == 16) ? Standard : Capture;
            moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],_square_from_index[resultingIndex],
                                      numberOfMoves+1, typeOfMove));
        }
    }

    if (enPassantSquare){ //The previous move made en passant possible
        int enPassantIndex = getIndicesOfBitsInBoard(enPassantSquare).at(0);
        if (colourToMove == White){
            if (index >= 32 && index <= 39){ //Only check for en passant if the pawn being examined is on the same row as the en passant square
                if ((_bit_masks[index]<<7) &enPassantSquare){
                    if (pawnsOnAdjacentColumns(enPassantIndex, index))
                        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],
                                                  _square_from_index[index+7], numberOfMoves+1, EnPassant));
                }
                else if ((_bit_masks[index]<<9) &enPassantSquare){
                    if (pawnsOnAdjacentColumns(enPassantIndex, index))
                        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],
                                              _square_from_index[index+9], numberOfMoves+1, EnPassant));
                }
            }
        }
        else{
            if (index >= 24 && index <= 31){ //Only check for en passant if the pawn being examined is on the same row as the en passant square
                if ((_bit_masks[index]>>7) &enPassantSquare){
                    if (pawnsOnAdjacentColumns(enPassantIndex, index))
                        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],
                                              _square_from_index[index-7], numberOfMoves+1, EnPassant));
                }
                else if ((_bit_masks[index]>>9) &enPassantSquare){
                    if (pawnsOnAdjacentColumns(enPassantIndex, index))
                        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Pawn), _square_from_index[index],
                                              _square_from_index[index-9], numberOfMoves+1, EnPassant));
                }
            }
        }
    }
    return moveVector;
}

vector<Move> ChessRules::getBitBoardPseudoMovesForKing(int index, BitBoard board, Colour colourToMove, int numberOfMoves){
    ULL possibleAttacks = _king_move_set[index];
    ULL pseudoLegalMoves = colourToMove == White ? possibleAttacks &~board._all_white_pieces : possibleAttacks &~board._all_black_pieces;

    //TODO: castling

    vector<Move> moveVector;
    for (auto resultingIndex : getIndicesOfBitsInBoard(pseudoLegalMoves)){
        MoveType moveType = _bit_masks[resultingIndex] & (colourToMove == White ? board._all_black_pieces : board._all_white_pieces) ? Capture : Standard;
        moveVector.push_back(Move(colourToMove, Piece(colourToMove, King), _square_from_index[index],_square_from_index[resultingIndex],
                                  numberOfMoves+1, moveType));
    }

    return moveVector;
}

vector<Move> ChessRules::getBitBoardCastlingMoves(BitBoard board, CastlingInfo castlingInfo, Colour colourToMove, int numberOfMoves){
    vector<Move> moveVector;
    if (colourToMove == White){
        if (!castlingInfo._white_castled && !castlingInfo._white_king_has_moved){
            if (!castlingInfo._white_short_rook_has_moved){
                bool threat = bitBoardSquareIsUnderAttack(_index_from_square["e1"], board, Black);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["f1"], board, Black);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["g1"], board, Black);
                bool blockedPath = board._all_pieces & (_bit_masks[_index_from_square["f1"]] | _bit_masks[_index_from_square["g1"]]);
                if (!threat && !blockedPath)
                    moveVector.push_back(Move(colourToMove, Piece(White, King), "e1", "g1", numberOfMoves+1, ShortCastle));
            }
            if (!castlingInfo._white_long_rook_has_moved){
                bool threat = bitBoardSquareIsUnderAttack(_index_from_square["e1"], board, Black);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["d1"], board, Black);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["c1"], board, Black);
                bool blockedPath = board._all_pieces & (_bit_masks[_index_from_square["d1"]] | _bit_masks[_index_from_square["c1"]] | _bit_masks[_index_from_square["b1"]]);
                if (!threat && !blockedPath)
                    moveVector.push_back(Move(colourToMove, Piece(White, King), "e1", "c1", numberOfMoves+1, LongCastle));
            }
        }
    }
    else{
        if (!castlingInfo._black_castled && !castlingInfo._black_king_has_moved){
            if (!castlingInfo._black_short_rook_has_moved){
                bool threat = bitBoardSquareIsUnderAttack(_index_from_square["e8"], board, White);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["f8"], board, White);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["g8"], board, White);
                bool blockedPath = board._all_pieces & (_bit_masks[_index_from_square["f8"]] | _bit_masks[_index_from_square["g8"]]);
                if (!threat && !blockedPath)
                    moveVector.push_back(Move(colourToMove, Piece(Black, King), "e8", "g8", numberOfMoves+1, ShortCastle));
            }
            if (!castlingInfo._black_long_rook_has_moved){
                bool threat = bitBoardSquareIsUnderAttack(_index_from_square["e8"], board, White);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["d8"], board, White);
                threat = threat || bitBoardSquareIsUnderAttack(_index_from_square["c8"], board, White);
                bool blockedPath = board._all_pieces & (_bit_masks[_index_from_square["d8"]] | _bit_masks[_index_from_square["c8"]] | _bit_masks[_index_from_square["b8"]]);
                if (!threat && !blockedPath)
                    moveVector.push_back(Move(colourToMove, Piece(Black, King), "e8", "c8", numberOfMoves+1, LongCastle));
            }
        }
    }
    int a = 1;
    return moveVector;
}

map<Piece, vector<int> > ChessRules::getIndicesOfBitsForPieceTypes(BitBoard board){
    map<Piece, vector<int>> returnMap;
    returnMap[Piece(White, Pawn)] = getIndicesOfBitsInBoard(board._white_pawns);
    returnMap[Piece(White, Rook)] = getIndicesOfBitsInBoard(board._white_rooks);
    returnMap[Piece(White, Knight)] = getIndicesOfBitsInBoard(board._white_knights);
    returnMap[Piece(White, Bishop)] = getIndicesOfBitsInBoard(board._white_bishops);
    returnMap[Piece(White, Queen)] = getIndicesOfBitsInBoard(board._white_queens);
    returnMap[Piece(White, King)] = getIndicesOfBitsInBoard(board._white_king);
    returnMap[Piece(Black, Pawn)] = getIndicesOfBitsInBoard(board._black_pawns);
    returnMap[Piece(Black, Rook)] = getIndicesOfBitsInBoard(board._black_rooks);
    returnMap[Piece(Black, Knight)] = getIndicesOfBitsInBoard(board._black_knights);
    returnMap[Piece(Black, Bishop)] = getIndicesOfBitsInBoard(board._black_bishops);
    returnMap[Piece(Black, Queen)] = getIndicesOfBitsInBoard(board._black_queens);
    returnMap[Piece(Black, King)] = getIndicesOfBitsInBoard(board._black_king);
    return returnMap;
}

ULL ChessRules::getBitBoardOfPossibleAttacksForBishop(int index, ULL occupancy){
    //Positive directions
    int lsb = getIndexOfLeastSignificantBit(_bishop_square_attack_rays[index][NE] & occupancy);
    ULL NEMoves = lsb == -1 ? _bishop_square_attack_rays[index][NE] : _bishop_square_attack_rays[index][NE] & _filled_up_to_masks[lsb];
    lsb = getIndexOfLeastSignificantBit(_bishop_square_attack_rays[index][NW] & occupancy);
    ULL NWMoves = lsb == -1 ? _bishop_square_attack_rays[index][NW] : _bishop_square_attack_rays[index][NW] & _filled_up_to_masks[lsb];

    //Negative directions
    int msb = getIndexOfMostSignificantBit(_bishop_square_attack_rays[index][SE] & occupancy);
    ULL SEMoves = msb == 0 ? _bishop_square_attack_rays[index][SE] : _bishop_square_attack_rays[index][SE] & _filled_down_to_masks[msb];
    msb = getIndexOfMostSignificantBit(_bishop_square_attack_rays[index][SW] & occupancy);
    ULL SWMoves = msb == 0 ? _bishop_square_attack_rays[index][SW] : _bishop_square_attack_rays[index][SW] & _filled_down_to_masks[msb];
    return NEMoves | NWMoves | SEMoves | SWMoves;
}

ULL ChessRules::getBitBoardOfPossibleAttacksForRook(int index, ULL occupancy){
    //Positive directions
    int lsb = getIndexOfLeastSignificantBit(_rook_square_attack_rays[index][N] & occupancy);
    ULL NMoves = lsb == -1 ? _rook_square_attack_rays[index][N] : _rook_square_attack_rays[index][N] & _filled_up_to_masks[lsb];
    lsb = getIndexOfLeastSignificantBit(_rook_square_attack_rays[index][E] & occupancy);
    ULL EMoves = lsb == -1 ? _rook_square_attack_rays[index][E] : _rook_square_attack_rays[index][E] & _filled_up_to_masks[lsb];

    //Negative directions
    int msb = getIndexOfMostSignificantBit(_rook_square_attack_rays[index][S] & occupancy);
    ULL SMoves = msb == 0 ? _rook_square_attack_rays[index][S] : _rook_square_attack_rays[index][S] & _filled_down_to_masks[msb];
    msb = getIndexOfMostSignificantBit(_rook_square_attack_rays[index][W] & occupancy);
    ULL WMoves = msb == 0 ? _rook_square_attack_rays[index][W] : _rook_square_attack_rays[index][W] & _filled_down_to_masks[msb];
    return NMoves | EMoves | SMoves | WMoves;
}

bool ChessRules::pawnsOnAdjacentColumns(int indexFirst, int indexSecond){
    return abs((indexSecond % 8) - (indexFirst % 8)) == 1;
}


//Not currently used. Replace with bitboard version
bool ChessRules::isInsufficientMaterial(State *state){
    int whiteMinorPieceCount = 0;
    int blackMinorPieceCount = 0;
    vector<pair<Piece, Colour>> minorPieces; //second element denotes colour of square
    for (int i = 0; i < state->_board_for_graphics.size(); i++){
        for (int j = 0; j < state->_board_for_graphics.at(i).size(); j++){
            if (state->_board_for_graphics.at(i).at(j) != nullptr){
                //Not insufficient material if pawn, queen, or rook is still in play
                if (state->_board_for_graphics.at(i).at(j)->_type == Queen ||
                        state->_board_for_graphics.at(i).at(j)->_type == Rook ||
                        state->_board_for_graphics.at(i).at(j)->_type == Pawn)
                    return false;
                else if (state->_board_for_graphics.at(i).at(j)->_type == Bishop || state->_board_for_graphics.at(i).at(j)->_type == Knight){
                    minorPieces.push_back(make_pair<Piece, Colour>(
                                              Piece(state->_board_for_graphics.at(i).at(j)->_colour, state->_board_for_graphics.at(i).at(j)->_type),
                                              i+j % 2 == 0 ? Black : White));
                    if (state->_board_for_graphics.at(i).at(j)->_colour == White)
                        whiteMinorPieceCount += 1;
                    if (state->_board_for_graphics.at(i).at(j)->_colour == Black)
                        blackMinorPieceCount += 1;
                }
            }
            if (whiteMinorPieceCount > 1 || blackMinorPieceCount > 1)
                return false;
        }
    }
    if ((whiteMinorPieceCount == 1 && blackMinorPieceCount == 0)
            || (blackMinorPieceCount == 1 && whiteMinorPieceCount == 0) ||
                (whiteMinorPieceCount == 0 && blackMinorPieceCount == 0))
        return true;
    if (blackMinorPieceCount == 1 && whiteMinorPieceCount == 1){
        //Insufficient if the only two minor pieces left are bishops on the same colour square
        if (minorPieces.at(0).first._type == Bishop &&
                minorPieces.at(1).first._type == Bishop &&
                minorPieces.at(0).second == minorPieces.at(1).second)
            return true;
    }
    return false;
}

int ChessRules::bitBoardNumberOfTimesThisStateSeen(ULL hash, map<ULL, int> *stateSeenCount){
    if (stateSeenCount->count(hash) > 0)
        stateSeenCount->find(hash)->second += 1;
    else
        stateSeenCount->insert(pair<ULL, int>(hash, 1));
    return stateSeenCount->find(hash)->second;
}

State* ChessRules::stateFromFEN(string fen){
    State *state = new State();
    state->_previous_state = nullptr;
    state->_bit_board_state_seen_count = new map<ULL, int>;
    state->_state_seen_count = new map<string, int>;

    vector<string> splitFen = splitString(fen, " ");

    string posString = splitFen.at(0);
    string colourToMoveString = splitFen.at(1);
    string castlingString = splitFen.at(2);
    string enPassantString = splitFen.at(3);
    state->_moves_without_capture_or_pawn_advancement = 0;
    state->_number_of_moves = 0;


    state->_colour_to_move = colourToMoveString == "w" ? White : Black;

    state->_castling_info._white_short_rook_has_moved = (castlingString.find('K') == string::npos);
    state->_castling_info._white_long_rook_has_moved = (castlingString.find('Q') == string::npos);
    state->_castling_info._white_king_has_moved = state->_castling_info._white_short_rook_has_moved &&
            state->_castling_info._white_long_rook_has_moved;
    state->_castling_info._white_castled = state->_castling_info._white_king_has_moved;
    state->_castling_info._black_short_rook_has_moved = (castlingString.find('k') == string::npos);
    state->_castling_info._black_long_rook_has_moved = (castlingString.find('q') == string::npos);
    state->_castling_info._black_king_has_moved = state->_castling_info._black_short_rook_has_moved &&
            state->_castling_info._black_long_rook_has_moved;
    state->_castling_info._black_castled = state->_castling_info._black_king_has_moved;


    qDebug() << state->_bit_board._en_passant_square;
    state->_bit_board._en_passant_square = enPassantString == "-" ? 0ULL : _bit_masks[_index_from_square[enPassantString]];
    qDebug() << state->_bit_board._en_passant_square;
    qDebug() << &enPassantString;


    if (state->_bit_board._en_passant_square){
        state->_move_to_state._piece._type = Pawn;
        state->_move_to_state._move_type = Standard;
        if (state->_colour_to_move == White){
            state->_move_to_state._origin_square = _square_from_index[ _index_from_square[enPassantString] + 8 ];
            state->_move_to_state._destination_square = _square_from_index[ _index_from_square[enPassantString] - 8 ];
        }
        else{
            state->_move_to_state._origin_square = _square_from_index[ _index_from_square[enPassantString] - 8 ];
            state->_move_to_state._destination_square = _square_from_index[ _index_from_square[enPassantString] + 8 ];
        }

    }
    vector<string> splitRows = splitString(posString, "/");
    reverse(splitRows.begin(), splitRows.end()); //Put row 1 first

    state->_bit_board._white_king = 0ULL;
    state->_bit_board._white_bishops = 0ULL;
    state->_bit_board._white_knights = 0ULL;
    state->_bit_board._white_pawns = 0ULL;
    state->_bit_board._white_queens = 0ULL;
    state->_bit_board._white_rooks = 0ULL;
    state->_bit_board._black_king = 0ULL;
    state->_bit_board._black_bishops = 0ULL;
    state->_bit_board._black_knights = 0ULL;
    state->_bit_board._black_pawns = 0ULL;
    state->_bit_board._black_queens = 0ULL;
    state->_bit_board._black_rooks = 0ULL;


    int currentIndex = -1;
    for (auto rowString: splitRows){
        for (auto squareChar: rowString){
            if (isdigit(squareChar)){
                currentIndex += (squareChar - '0'); //casts squareChar to int
            }
            else{
                currentIndex++;
                switch (squareChar) {
                case 'K' :
                    state->_bit_board._white_king |= _bit_masks[currentIndex];
                    break;
                case 'Q' :
                    state->_bit_board._white_queens |= _bit_masks[currentIndex];
                    break;
                case 'B' :
                    state->_bit_board._white_bishops |= _bit_masks[currentIndex];
                    break;
                case 'N' :
                    state->_bit_board._white_knights |= _bit_masks[currentIndex];
                    break;
                case 'R' :
                    state->_bit_board._white_rooks |= _bit_masks[currentIndex];
                    break;
                case 'P' :
                    state->_bit_board._white_pawns |= _bit_masks[currentIndex];
                    break;

                case 'k' :
                    state->_bit_board._black_king |= _bit_masks[currentIndex];
                    break;
                case 'q' :
                    state->_bit_board._black_queens |= _bit_masks[currentIndex];
                    break;
                case 'b' :
                    state->_bit_board._black_bishops |= _bit_masks[currentIndex];
                    break;
                case 'n' :
                    state->_bit_board._black_knights |= _bit_masks[currentIndex];
                    break;
                case 'r' :
                    state->_bit_board._black_rooks |= _bit_masks[currentIndex];
                    break;
                case 'p' :
                    state->_bit_board._black_pawns |= _bit_masks[currentIndex];
                    break;
                }
            }
        }
    }
    state->_bit_board._all_white_pieces = state->_bit_board._white_king | state->_bit_board._white_bishops |
            state->_bit_board._white_queens | state->_bit_board._white_knights | state->_bit_board._white_rooks |
            state->_bit_board._white_pawns;
    state->_bit_board._all_black_pieces = state->_bit_board._black_king | state->_bit_board._black_bishops |
            state->_bit_board._black_queens | state->_bit_board._black_knights | state->_bit_board._black_rooks |
            state->_bit_board._black_pawns;
    state->_bit_board._all_pieces = state->_bit_board._all_white_pieces | state->_bit_board._all_black_pieces;

    state->_white_king_is_in_check = state->_colour_to_move == Black ?
                bitBoardWhiteKingIsInCheck(state->_bit_board) : false;
    state->_black_king_is_in_check = state->_colour_to_move == White ?
                bitBoardBlackKingIsInCheck(state->_bit_board) : false;


    state->_position_hash = _hasher.generateHashForPosition(state->_indices_of_bits_for_piece_types,
                                                            state->_castling_info,
                                                            state->_colour_to_move,
                                                            getIndicesOfBitsInBoard(state->_bit_board._en_passant_square));


    vector<Move> legalMoves = getLegalBitBoardMoves(state);
    if (legalMoves.size() == 0)
        state->_is_game_over = true;
    state->_black_won = state->_white_king_is_in_check && state->_is_game_over;
    state->_white_won = state->_black_king_is_in_check && state->_is_game_over;
    state->_is_draw = !((state->_white_won || state->_black_won) && state->_is_game_over);

    state->_legal_moves_from_state = legalMoves;

    return state;
}

void ChessRules::runPerftTest(State *state, int maxDepth, bool printDivide){
    map<int, int> *movePerDepthCounter = new map<int, int>();
    movePerDepthCounter->insert_or_assign(0, 1);

    map<string, int> *divideMap = printDivide ? new map<string, int>() : nullptr;

    map<string, int> *moveTypeCounter = new map<string, int>();
    moveTypeCounter->insert_or_assign("captures", 0);
    moveTypeCounter->insert_or_assign("en_passants", 0);
    moveTypeCounter->insert_or_assign("castles", 0);
    moveTypeCounter->insert_or_assign("promotions", 0);
    moveTypeCounter->insert_or_assign("checks", 0);
    moveTypeCounter->insert_or_assign("check_mates", 0);

    expandPerftTree(state, movePerDepthCounter, 0, maxDepth, printDivide, moveTypeCounter, divideMap);

    if (movePerDepthCounter->size() > 0){
        cout << endl;
        for (map<int, int>::iterator it = movePerDepthCounter->begin(); it != movePerDepthCounter->end(); ++it)
            cout << "Perft depth " << it->first << ": " << it->second << endl;
    }

    cout << endl;
    for (map<string, int>::iterator it = moveTypeCounter->begin(); it != moveTypeCounter->end(); ++it)
        cout << it->first << ": " << it->second << endl;
    cout << endl;

    if (printDivide){
        if (divideMap->size() > 0){
            cout << "Divide:" << endl << endl;
            for (map<string, int>::iterator it = divideMap->begin(); it != divideMap->end(); ++it)
                cout << it->first << ": " << it->second << endl;
        }
        cout << endl;
    }

    //cout << "Test completed in " << testTimer.elapsed() / 1000.0 << " seconds." << endl;
}

void ChessRules::expandPerftTree(State *currentState, map<int, int> *movePerDepthCounter, int currentDepth,
                                 int maxDepth, bool printDivide, map<string, int> *moveTypeCounter,
                                 map<string, int> *divideMap, string divideString){

    vector<Move> legalMoves;
    if (currentDepth < maxDepth)
        legalMoves = getLegalBitBoardMoves(currentState);

    if (printDivide && currentDepth == 0){
        for (auto move: legalMoves){
            string str = "";
            str += move._origin_square;
            str += move._destination_square;
            divideMap->insert_or_assign(str, 0);
        }
    }
    if (currentDepth == 0)
        movePerDepthCounter->insert(pair<int, int>(currentDepth, 1));
    else{
        if (movePerDepthCounter->count(currentDepth) > 0)
            movePerDepthCounter->find(currentDepth)->second++;
        else
            movePerDepthCounter->insert(pair<int, int>(currentDepth, 1));
    }

    if (printDivide && currentDepth == maxDepth){
        divideMap->find(divideString)->second++;
    }

    for (auto move: legalMoves){
        State *resultingState = getResultingStateFromMove(currentState, move);
        if (printDivide && currentDepth == 0){
            divideString = "";
            divideString += move._origin_square;
            divideString += move._destination_square;
        }
        if (resultingState->_move_to_state._move_type != Standard){
            MoveType type = resultingState->_move_to_state._move_type;
            if (type == ShortCastle || type == LongCastle)
                moveTypeCounter->find("castles")->second++;
            if (type == Capture || type == PromotionCapture)
                moveTypeCounter->find("captures")->second++;
            if (type == EnPassant)
                moveTypeCounter->find("en_passants")->second++;
            if (type == Promotion || type == PromotionCapture)
                moveTypeCounter->find("promotions")->second++;
            if (resultingState->_black_king_is_in_check || resultingState->_white_king_is_in_check){
                if (resultingState->_is_game_over)
                    moveTypeCounter->find("check_mates")->second++;
                else
                    moveTypeCounter->find("checks")->second++;
            }
        }

        if (currentDepth < maxDepth){
            expandPerftTree(resultingState, movePerDepthCounter, currentDepth+1, maxDepth, printDivide,
                            moveTypeCounter, divideMap, divideString);
        }
        delete resultingState;
    }
}










