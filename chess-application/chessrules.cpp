#include "chessrules.h"

ChessRules::ChessRules(){
    _accumulated_test_time = 0;
}

vector<Move> ChessRules::getLegalMoves(State *state, PieceType promotionPiece, bool checkIfCheck){

    vector<Move> legalMoves;
    Colour colourToMove = state->_colour_to_move;
    int numberOfMoves = state->_number_of_moves;
    vector<vector<Piece*>> board = state->_board;

    for(int i = 0; i < board.size(); i++){
        for (int j = 0; j < board.at(i).size(); j++){
            if (board.at(i).at(j) != nullptr){
                if (board.at(i).at(j)->_colour == colourToMove){

                    if (board.at(i).at(j)->_type == Rook){
                        vector<Move> rookMoves = getMovesForRook(board, colourToMove, i, j, numberOfMoves, Rook);
                        legalMoves.insert(legalMoves.end(), rookMoves.begin(), rookMoves.end());
                    }
                    else if (board.at(i).at(j)->_type == Bishop){
                        vector<Move> bishopMoves = getMovesForBishop(board, colourToMove, i, j, numberOfMoves, Bishop);
                        legalMoves.insert(legalMoves.end(), bishopMoves.begin(), bishopMoves.end());
                    }
                    else if (board.at(i).at(j)->_type == Queen){
                        vector<Move> queenMoves = getMovesForQueen(board, colourToMove, i, j, numberOfMoves, Queen);
                        legalMoves.insert(legalMoves.end(), queenMoves.begin(), queenMoves.end());
                    }
                    else if (board.at(i).at(j)->_type == Knight){
                        vector<Move> knightMoves = getMovesForKnight(board, colourToMove, i, j, numberOfMoves, Knight);
                        legalMoves.insert(legalMoves.end(), knightMoves.begin(), knightMoves.end());
                    }
                    else if (board.at(i).at(j)->_type == Pawn){
                        vector<Move> pawnMoves = getMovesForPawn(board, colourToMove, i, j, numberOfMoves, Pawn, state->_move_to_state);
                        legalMoves.insert(legalMoves.end(), pawnMoves.begin(), pawnMoves.end());
                    }
                    else if (board.at(i).at(j)->_type == King){
                        vector<Move> kingMoves = getMovesForKing(state, board, colourToMove, i, j, numberOfMoves, King, state->_castling_info, checkIfCheck);
                        legalMoves.insert(legalMoves.end(), kingMoves.begin(), kingMoves.end());
                    }

                }
            }
        }
    }


    //checkIfCheck is true when checking for legal moves, false when checking for which squares are under attack
    if (checkIfCheck)
        legalMoves = checkIfMovesCauseCheckForSelf(legalMoves, board, colourToMove, state, promotionPiece);


    return legalMoves;

}

vector<Move> ChessRules::checkIfMovesCauseCheckForSelf(vector<Move> movesToCheck, vector<vector<Piece*>> board, Colour colourToMove, State *state, PieceType promotionPiece){
    vector<Move> nonCheckMoves;

    for (auto move: movesToCheck){
        string originSquare = move._origin_square;
        string destinationSquare = move._destination_square;
        int rowFrom = IndicesFromSquareID(originSquare).first;
        int colFrom = IndicesFromSquareID(originSquare).second;
        int rowTo = IndicesFromSquareID(destinationSquare).first;
        int colTo = IndicesFromSquareID(destinationSquare).second;
        Piece pieceToMove = move._piece;
        State *resultingState = new State();
        resultingState->_colour_to_move = move._colour_performing_move == White ? Black : White;
        resultingState->_move_to_state = move;
        resultingState->_previous_state = state;
        resultingState->_castling_info = state->_castling_info;
        for (int i = 0; i < state->_board.size(); i++){
            for (int j = 0; j < state->_board.at(i).size(); j++){
                resultingState->_board.at(i).at(j) = state->_board.at(i).at(j);
            }
        }
        resultingState->_board.at(rowFrom).at(colFrom) = nullptr;
        resultingState->_board.at(rowTo).at(colTo) = new Piece(pieceToMove._colour, pieceToMove._type);
        if (move._move_type == LongCastle || move._move_type == ShortCastle)
            performCheckCheckCastlingMove(move, resultingState);
        if (move._move_type == Promotion || move._move_type == PromotionCapture)
            resultingState->_board.at(rowTo).at(colTo) = new Piece(pieceToMove._colour, promotionPiece);
        if (move._move_type == Capture || move._move_type == PromotionCapture || move._move_type == EnPassant)
            resultingState->_moves_without_capture_or_pawn_advancement = 0;
        if (move._move_type == EnPassant)
            performCheckCheckEnPassantMove(move, resultingState);

        string kingSquare;
        if (move._piece._type == King){
            kingSquare = move._destination_square;
        }
        else{
            for (int i = 0; i < state->_board.size(); i++){
                for (int j = 0; j < state->_board.at(i).size(); j++){
                    if (board.at(i).at(j) != nullptr){
                        if (board.at(i).at(j)->_type == King && board.at(i).at(j)->_colour == move._colour_performing_move)
                            kingSquare = squareIDFromIndices(i, j);
                    }
                }
            }
        }
        vector<Move> resultingLegalMoves = getLegalMoves(resultingState, promotionPiece, false);
        bool moveCausedCheck = false;
        for (auto resultingMove: resultingLegalMoves){
            if (resultingMove._destination_square == kingSquare){
                moveCausedCheck = true;
                break;
            }
        }
        if (!moveCausedCheck)
            nonCheckMoves.push_back(move);
    }
    return nonCheckMoves;
}


State* ChessRules::getResultingStateFromMove(State *currentState, Move moveToMake){

    currentState->_move_from_state = moveToMake;

    int rowFrom = IndicesFromSquareID(moveToMake._origin_square).first;
    int colFrom = IndicesFromSquareID(moveToMake._origin_square).second;
    int rowTo = IndicesFromSquareID(moveToMake._destination_square).first;
    int colTo = IndicesFromSquareID(moveToMake._destination_square).second;

    Piece pieceToMove = moveToMake._piece;

    State *resultingState = new State();
    resultingState->_state_seen_count = currentState->_state_seen_count;
    resultingState->_colour_to_move = currentState->_colour_to_move == White ? Black : White;
    resultingState->_move_to_state = moveToMake;
    resultingState->_previous_state = currentState;
    resultingState->_number_of_moves = currentState->_number_of_moves+1;
    resultingState->_moves_without_capture_or_pawn_advancement = (moveToMake._move_type == Capture || moveToMake._piece._type == Pawn) ? 0 : currentState->_moves_without_capture_or_pawn_advancement+1;
    resultingState->_castling_info = currentState->_castling_info;

    for (int i = 0; i < currentState->_board.size(); i++){
        for (int j = 0; j < currentState->_board.at(i).size(); j++){
            resultingState->_board.at(i).at(j) = currentState->_board.at(i).at(j);
        }
    }

    resultingState->_board.at(rowFrom).at(colFrom) = nullptr;
    resultingState->_board.at(rowTo).at(colTo) = new Piece(pieceToMove._colour, pieceToMove._type);


    //Handle castling info and moves
    updateCastlingInfo(moveToMake, resultingState);
    if (moveToMake._move_type == LongCastle || moveToMake._move_type == ShortCastle)
        performCastlingMove(moveToMake, resultingState);
    if (moveToMake._move_type == Promotion || moveToMake._move_type == PromotionCapture)
        resultingState->_board.at(rowTo).at(colTo) = new Piece(pieceToMove._colour, moveToMake._promotion_selection);
    if (moveToMake._move_type == EnPassant)
        performEnPassantMove(moveToMake, resultingState);

    _test_timer.start();
    resultingState->_legal_moves_from_state = getLegalMoves(resultingState, moveToMake._promotion_selection);
    vector<Move> moves = getLegalBitBoardMoves(resultingState);
    _accumulated_test_time += _test_timer.elapsed();

    currentState->_next_state = resultingState;
    currentState = resultingState;
    currentState->_white_king_is_in_check = whiteKingIsInCheck(currentState);
    currentState->_black_king_is_in_check = blackKingIsInCheck(currentState);
    if (currentState->_white_king_is_in_check || currentState->_black_king_is_in_check)
        currentState->_square_under_check = _square_under_check;
    _square_under_check = _square_under_check;


    setFenForState(currentState);
    if (currentState->_legal_moves_from_state.size() == 0){ //End the game if there are no legal moves
        currentState->_is_game_over = true;

        if (currentState->_colour_to_move == Black && currentState->_black_king_is_in_check){
            resultingState->_white_won = true;
            resultingState->_game_over_reason = "Check Mate";
        }
        else if (currentState->_colour_to_move == White && currentState->_white_king_is_in_check){
            resultingState->_black_won = true;
            resultingState->_game_over_reason = "Check Mate";
        }
        else{
            resultingState->_is_draw = true;
            resultingState->_game_over_reason = "Stalemate";
        }
    }
    if (isInsufficientMaterial(currentState)){
        currentState->_is_game_over = true;
        resultingState->_is_draw = true;
        resultingState->_game_over_reason = "Insufficient mating material";
    }
    if (currentState->_moves_without_capture_or_pawn_advancement >= 100){
        currentState->_is_game_over = true;
        resultingState->_is_draw = true;
        resultingState->_game_over_reason = "50 move rule";
    }

    if (moveToMake._piece._type == Pawn)
        resultingState->_state_seen_count->clear();
    if (numberOfTimesThisStateSeen(currentState->_fen_notation, resultingState->_state_seen_count) >= 3){
        currentState->_is_game_over = true;
        resultingState->_is_draw = true;
        resultingState->_game_over_reason = "Threefold repetition";
    }


    resultingState->_is_game_over = currentState->_is_game_over;

    return currentState;
}

void ChessRules::updateCastlingInfo(Move move, State *state){
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

void ChessRules::performCastlingMove(Move move, State *state){
    if (move._move_type == LongCastle){
        if (move._colour_performing_move == White){
            state->_board.at(0).at(0) = nullptr;
            state->_board.at(0).at(4) = nullptr;
            state->_board.at(0).at(2) = new Piece(White, King);
            state->_board.at(0).at(3) = new Piece(White, Rook);
        }
        else{
            state->_board.at(7).at(0) = nullptr;
            state->_board.at(7).at(4) = nullptr;
            state->_board.at(7).at(2) = new Piece(Black, King);
            state->_board.at(7).at(3) = new Piece(Black, Rook);
        }
    }
    if (move._move_type == ShortCastle){
        if (move._colour_performing_move == White){
            state->_board.at(0).at(7) = nullptr;
            state->_board.at(0).at(4) = nullptr;
            state->_board.at(0).at(6) = new Piece(White, King);
            state->_board.at(0).at(5) = new Piece(White, Rook);
        }
        else{
            state->_board.at(7).at(7) = nullptr;
            state->_board.at(7).at(4) = nullptr;
            state->_board.at(7).at(6) = new Piece(Black, King);
            state->_board.at(7).at(5) = new Piece(Black, Rook);
        }
    }
}

void ChessRules::performEnPassantMove(Move move, State *state){
    int rowFrom = IndicesFromSquareID(move._origin_square).first;
    int colTo = IndicesFromSquareID(move._destination_square).second;
    state->_board.at(rowFrom).at(colTo) = nullptr;
}

void ChessRules::setFenForState(State *state){
    string fenBuilder = "";
    for (int i = state->_board.size()-1; i >= 0; i--){
        int emptySquareCounter = 0;
        bool pieceFoundOnLastSquare = false;
        for (int j = 0; j < state->_board.size(); j++){
            if (state->_board.at(i).at(j) == nullptr){
                emptySquareCounter++;
            }
            else{
                if (j == 7)
                   pieceFoundOnLastSquare = true;
                if (emptySquareCounter > 0){
                    fenBuilder += to_string(emptySquareCounter);
                }
                emptySquareCounter = 0;
                if (state->_board.at(i).at(j)->_type == Pawn)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'P' : 'p');
                else if (state->_board.at(i).at(j)->_type == Rook)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'R' : 'r');
                else if (state->_board.at(i).at(j)->_type == Bishop)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'B' : 'b');
                else if (state->_board.at(i).at(j)->_type == Knight)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'N' : 'n');
                else if (state->_board.at(i).at(j)->_type == Queen)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'Q' : 'q');
                else if (state->_board.at(i).at(j)->_type == King)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'K' : 'k');
            }
        }
        if (!pieceFoundOnLastSquare)
            fenBuilder += to_string(emptySquareCounter);
        if (i > 0)
            fenBuilder.push_back('/');
    }
    fenBuilder.push_back(' ');
    fenBuilder.push_back(state->_colour_to_move == White ? 'w' : 'b');
    fenBuilder.push_back(' ');
    if ((state->_castling_info._white_castled || state->_castling_info._white_king_has_moved) &&
            (state->_castling_info._black_castled || state->_castling_info._black_king_has_moved)){
        fenBuilder.push_back('-');
    }
    else{
        if (!state->_castling_info._white_king_has_moved && !state->_castling_info._white_short_rook_has_moved)
            fenBuilder.push_back('K');
        if (!state->_castling_info._white_king_has_moved && !state->_castling_info._white_long_rook_has_moved)
            fenBuilder.push_back('Q');
        if (!state->_castling_info._black_king_has_moved && !state->_castling_info._black_short_rook_has_moved)
            fenBuilder.push_back('k');
        if (!state->_castling_info._black_king_has_moved && !state->_castling_info._black_long_rook_has_moved)
            fenBuilder.push_back('q');
    }
    fenBuilder.push_back(' ');
    for (auto strChar: enPassantTargetSquareForFEN(state->_move_to_state))
        fenBuilder.push_back(strChar);
    fenBuilder.push_back(' ');
    fenBuilder += to_string(state->_moves_without_capture_or_pawn_advancement);
    fenBuilder.push_back(' ');
    fenBuilder += to_string(int((state->_number_of_moves+2) / 2));
    state->_fen_notation = fenBuilder;
}

string ChessRules::enPassantTargetSquareForFEN(Move move){
    if (move._piece._type != Pawn)
        return "-";
    int rowFrom = IndicesFromSquareID(move._origin_square).first;
    int colFrom = IndicesFromSquareID(move._origin_square).second;
    int rowTo = IndicesFromSquareID(move._destination_square).first;
    int colTo = IndicesFromSquareID(move._destination_square).second;

    if (abs(rowFrom - rowTo) < 2)
        return "-";
    else{
        if (move._colour_performing_move == White)
            return squareIDFromIndices(rowTo-1, colTo);
        else
            return squareIDFromIndices(rowTo+1, colTo);
    }
}

void ChessRules::performCheckCheckEnPassantMove(Move move, State *state){
    int rowFrom = IndicesFromSquareID(move._origin_square).first;
    int colTo = IndicesFromSquareID(move._destination_square).second;
    state->_board.at(rowFrom).at(colTo) == nullptr;
}

void ChessRules::performCheckCheckCastlingMove(Move move, State *state){
    if (move._move_type == LongCastle){
        if (move._colour_performing_move == White){
            state->_board.at(0).at(0) = nullptr;
            state->_board.at(0).at(4) = nullptr;
            state->_board.at(0).at(2) = new Piece(White, King);
            state->_board.at(0).at(3) = new Piece(White, Rook);
        }
        else{
            state->_board.at(7).at(0) = nullptr;
            state->_board.at(7).at(4) = nullptr;
            state->_board.at(7).at(2) = new Piece(Black, King);
            state->_board.at(7).at(3) = new Piece(Black, Rook);
        }
    }
    if (move._move_type == ShortCastle){
        if (move._colour_performing_move == White){
            state->_board.at(0).at(7) = nullptr;
            state->_board.at(0).at(4) = nullptr;
            state->_board.at(0).at(6) = new Piece(White, King);
            state->_board.at(0).at(5) = new Piece(White, Rook);
        }
        else{
            state->_board.at(7).at(7) = nullptr;
            state->_board.at(7).at(4) = nullptr;
            state->_board.at(7).at(6) = new Piece(Black, King);
            state->_board.at(7).at(5) = new Piece(Black, Rook);
        }
    }
}

vector<Move> ChessRules::getMovesForKnight(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type){
    Colour opponentColour = colourToMove == White ? Black : White;
    vector<Move> knightMoves;
    string originID = squareIDFromIndices(row, col);
    for (auto move: _possible_knight_moves){
        int rowChange = move.first;
        int colChange = move.second;
        if (row+rowChange >= 0 && row+rowChange < 8 && col+colChange >= 0 && col+colChange < 8){
            if (board.at(row+rowChange).at(col+colChange) == nullptr){
                knightMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                         squareIDFromIndices(row+rowChange, col+colChange), previousMoveNumber+1));
            }
            else if (board.at(row+rowChange).at(col+colChange)->_colour == opponentColour){
                knightMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                           squareIDFromIndices(row+rowChange, col+colChange), previousMoveNumber+1, Capture));
            }
        }
    }
    return knightMoves;
}

vector<Move> ChessRules::getMovesForKing(State *state, vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type, CastlingInfo castlingInfo, bool checkIfCheck){
    Colour opponentColour = colourToMove == White ? Black : White;
    vector<Move> kingMoves;
    string originID = squareIDFromIndices(row, col);

    //Check standard king moves
    for (auto move: _possible_king_moves){
        int rowChange = move.first;
        int colChange = move.second;
        if (row+rowChange >= 0 && row+rowChange < 8 && col+colChange >= 0 && col+colChange < 8){
            if (board.at(row+rowChange).at(col+colChange) == nullptr){
                kingMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                         squareIDFromIndices(row+rowChange, col+colChange), previousMoveNumber+1));
            }
            else if (board.at(row+rowChange).at(col+colChange)->_colour == opponentColour){
                kingMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                         squareIDFromIndices(row+rowChange, col+colChange), previousMoveNumber+1, Capture));
            }
        }
    }

    //Check castling:
    //Do not check castling if checkIfCheck
    if (checkIfCheck && ((colourToMove == White && !castlingInfo._white_castled && !castlingInfo._white_king_has_moved) ||
            (colourToMove == Black && !castlingInfo._black_castled && !castlingInfo._black_king_has_moved))){

        bool longRookMoved;
        bool shortRookMoved;
        if (colourToMove == White){
            longRookMoved = castlingInfo._white_long_rook_has_moved;
            shortRookMoved = castlingInfo._white_short_rook_has_moved;
        }
        else{
            longRookMoved = castlingInfo._black_long_rook_has_moved;
            shortRookMoved = castlingInfo._black_short_rook_has_moved;
        }
        //Check long castling:
        if (!longRookMoved){
            //Check squares are unoccuppied and not under attack
            if (!squareIsUnderAttack(row, col, state) && !squareIsUnderAttack(row, col-1, state) && !squareIsUnderAttack(row, col-2, state) &&
                    (board.at(row).at(col-1) == nullptr) && (board.at(row).at(col-2) == nullptr) && (board.at(row).at(col-3) == nullptr)){
                kingMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                         squareIDFromIndices(row, col-2), previousMoveNumber+1, LongCastle));
            }
        }
        //Check short castling
        if (!shortRookMoved){
            //Check squares are unoccuppied and not under attack
            if (!squareIsUnderAttack(row, col, state) && !squareIsUnderAttack(row, col+1, state) && !squareIsUnderAttack(row, col+2, state) &&
                    (board.at(row).at(col+1) == nullptr) && (board.at(row).at(col+2) == nullptr)){
                kingMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                         squareIDFromIndices(row, col+2), previousMoveNumber+1, ShortCastle));
            }
        }
    }
    return kingMoves;
}

bool ChessRules::squareIsUnderAttack(int row, int col, State *state){
    //threateningMoves include moves that are illegal due to revealing check of own king, but that need to be considered here
    State *fakeState = new State();
    memcpy(fakeState, state, sizeof(State));
    fakeState->_colour_to_move = state->_colour_to_move == White ? Black : White;
    vector<Move> threateningMoves = getLegalMoves(fakeState, Queen, false);
    string squareID = squareIDFromIndices(row, col);
    for (auto move: threateningMoves)
        if (move._destination_square == squareID)
            return true;
    return false;
}

vector<Move> ChessRules::getMovesForPawn(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type, Move previousMove){
    Colour opponentColour = colourToMove == White ? Black : White;
    vector<Move> pawnMoves;
    string originID = squareIDFromIndices(row, col);

    //Check for moves for white
    if (colourToMove == White){
        //Check for moves straight forward
        if (board.at(row+1).at(col) == nullptr){ //Square in front is free, pawn can move forward
            if (row+1 == 7){ //Promotion
                pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                         squareIDFromIndices(row, col), squareIDFromIndices(row+1, col),
                                         previousMoveNumber+1, Promotion));
            }
            else{
                pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                         squareIDFromIndices(row, col), squareIDFromIndices(row+1, col),
                                         previousMoveNumber+1));
                if (row == 1 && board.at(row+2).at(col) == nullptr) //Pawn can move two steps
                    pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                             squareIDFromIndices(row, col), squareIDFromIndices(row+2, col),
                                             previousMoveNumber+1));
            }
        }
        //Check for capture moves
        if (col-1 >= 0)
            if (board.at(row+1).at(col-1) != nullptr)
                if (board.at(row+1).at(col-1)->_colour == opponentColour){
                    if (row+1 == 7)
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row+1, col-1),
                                                 previousMoveNumber+1, PromotionCapture));
                    else
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row+1, col-1),
                                                 previousMoveNumber+1, Capture));
                }

        if (col+1 < 8)
            if (board.at(row+1).at(col+1) != nullptr)
                if (board.at(row+1).at(col+1)->_colour == opponentColour){
                    if (row+1 == 7)
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row+1, col+1),
                                                 previousMoveNumber+1, PromotionCapture));
                    else
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row+1, col+1),
                                                 previousMoveNumber+1, Capture));
                }
    }
    //Check moves for black
    else{
        //Check for moves straight forward
        if (board.at(row-1).at(col) == nullptr){ //Square in front is free, pawn can move forward
            if (row-1 == 0){ //Promotion
                pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                         squareIDFromIndices(row, col), squareIDFromIndices(row-1, col),
                                         previousMoveNumber+1, Promotion));
            }
            else{
                pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                         squareIDFromIndices(row, col), squareIDFromIndices(row-1, col),
                                         previousMoveNumber+1));
                if (row == 6 && board.at(row-2).at(col) == nullptr) //Pawn can move two steps
                    pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                             squareIDFromIndices(row, col), squareIDFromIndices(row-2, col),
                                             previousMoveNumber+1));
            }
        }
        //Check for capture moves
        if (col-1 >= 0)
            if (board.at(row-1).at(col-1) != nullptr)
                if (board.at(row-1).at(col-1)->_colour == opponentColour){
                    if (row-1 == 0)
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row-1, col-1),
                                                 previousMoveNumber+1, PromotionCapture));
                    else
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row-1, col-1),
                                                 previousMoveNumber+1, Capture));
                }

        if (col+1 < 8)
            if (board.at(row-1).at(col+1) != nullptr)
                if (board.at(row-1).at(col+1)->_colour == opponentColour){
                    if (row-1 == 0)
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row-1, col+1),
                                                 previousMoveNumber+1, PromotionCapture));
                    else
                        pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                 squareIDFromIndices(row, col), squareIDFromIndices(row-1, col+1),
                                                 previousMoveNumber+1, Capture));
                }
    }


    //Check if en passant is possible
    if (previousMove._move_number > 0){ //Don't check for en passant if first move
        if (previousMove._piece._type == Pawn){
            //Check for en passant if previous move was a 2 step pawn move:
            if (abs(IndicesFromSquareID(previousMove._origin_square).first - IndicesFromSquareID(previousMove._destination_square).first) == 2){
                //Check if previous pawn was moved to same row as our pawn:
                if (IndicesFromSquareID(previousMove._destination_square).first == row){
                    //Check if previous pawn was moved on the column next to our pawn
                    if (IndicesFromSquareID(previousMove._destination_square).second - 1 == col){
                        if (colourToMove == White)
                            pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                     squareIDFromIndices(row, col), squareIDFromIndices(row+1, col+1),
                                                     previousMoveNumber+1, EnPassant));
                        else
                            pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                     squareIDFromIndices(row, col), squareIDFromIndices(row-1, col+1),
                                                     previousMoveNumber+1, EnPassant));
                    }
                    if (IndicesFromSquareID(previousMove._destination_square).second + 1 == col){
                        if (colourToMove == White)
                            pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                     squareIDFromIndices(row, col), squareIDFromIndices(row+1, col-1),
                                                     previousMoveNumber+1, EnPassant));
                        else
                            pawnMoves.push_back(Move(colourToMove, Piece(colourToMove, type),
                                                     squareIDFromIndices(row, col), squareIDFromIndices(row-1, col-1),
                                                     previousMoveNumber+1, EnPassant));
                    }
                }
            }
        }
    }
    return pawnMoves;
}

vector<Move> ChessRules::getLegalBitBoardMoves(State *state){
    vector<int> knightIndices = getIndicesOfBitsInBoard(state->_bit_board._white_knights);
    vector<Move> knightMoves;
    for (auto index: knightIndices)
        for (auto move: getBitBoardMovesForKnight(index, state->_bit_board, White, 0))
            knightMoves.push_back(move);
    return knightMoves;
}

vector<Move> ChessRules::getBitBoardMovesForKnight(int index, BitBoard board, Colour colourToMove, int numberOfMoves){
    ULL possibleAttacks = _knight_attack_set[index];
    ULL pseudoLegalMoves = colourToMove == White ? possibleAttacks &~board._all_white_pieces : possibleAttacks &~board._all_black_pieces;

    //printBoard(possibleAttacks);
    //printBoard(pseudoLegalMoves);

    //remove moves that cause check

    vector<Move> moveVector;
    for (auto resultingIndex : getIndicesOfBitsInBoard(pseudoLegalMoves))
        moveVector.push_back(Move(colourToMove, Piece(colourToMove, Knight), _square_from_index[index],_square_from_index[resultingIndex],
                                  numberOfMoves+1, Standard));
    return moveVector;
}

vector<Move> ChessRules::getMovesForQueen(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type){
    vector<Move> queenMoves;
    vector<Move> straightMoves;
    vector<Move> diagonalMoves;
    straightMoves = getMovesForRook(board, colourToMove, row, col, previousMoveNumber, type);
    diagonalMoves = getMovesForBishop(board, colourToMove, row, col, previousMoveNumber, type);
    queenMoves.insert(queenMoves.end(), straightMoves.begin(), straightMoves.end());
    queenMoves.insert(queenMoves.end(), diagonalMoves.begin(), diagonalMoves.end());
    return queenMoves;
}

vector<Move> ChessRules::getMovesForBishop(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type){
    Colour opponentColour = colourToMove == White ? Black : White;
    vector<Move> bishopMoves;
    string originID = squareIDFromIndices(row, col);

    for (int i = row, j = col; i < 8 && j < 8; i++, j++){
        if (i == row && j == col)
            continue;
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(j)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1, Capture);
            bishopMoves.push_back(move);
            break;
        }
        else
            break;
    }

    for (int i = row, j = col; i < 8 && j >= 0; i++, j--){
        if (i == row && j == col)
            continue;
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(j)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1, Capture);
            bishopMoves.push_back(move);
            break;
        }
        else
           break;
    }

    for (int i = row, j = col; i >= 0 && j < 8; i--, j++){
        if (i == row && j == col)
            continue;
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(j)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1, Capture);
            bishopMoves.push_back(move);
            break;
        }
        else
            break;
    }

    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--){
        if (i == row && j == col)
            continue;
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(j)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1, Capture);
            bishopMoves.push_back(move);
            break;
        }
        else
            break;
    }
    return bishopMoves;
}

vector<Move> ChessRules::getMovesForRook(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type){
    Colour opponentColour = colourToMove == White ? Black : White;
    vector<Move> rookMoves;
    string originID = squareIDFromIndices(row, col);
    for (int i = row; i < 8; i++){
        if (i == row)
            continue;
        if (board.at(i).at(col) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, col), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, col), previousMoveNumber+1, Capture);
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }
    for (int i = row; i >= 0; i--){
        if (i == row)
            continue;
        if (board.at(i).at(col) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, col), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, col), previousMoveNumber+1, Capture);
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }

    for (int i = col; i < 8; i++){
        if (i == col)
            continue;
        if (board.at(row).at(i) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(row, i), previousMoveNumber+1));
        }
        else if (board.at(row).at(i)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(row, i), previousMoveNumber+1, Capture);
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }
    for (int i = col; i >= 0; i--){
        if (i == col)
            continue;
        if (board.at(row).at(i) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(row, i), previousMoveNumber+1));
        }
        else if (board.at(row).at(i)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(row, i), previousMoveNumber+1, Capture);
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }
    return rookMoves;
}

bool ChessRules::whiteKingIsInCheck(State *state){
    string whiteKingSquare;
    for (int i = 0; i < state->_board.size(); i++){
        for (int j = 0; j < state->_board.at(i).size(); j++){
            if (state->_board.at(i).at(j) != nullptr){
                if (state->_board.at(i).at(j)->_type == King &&
                        state->_board.at(i).at(j)->_colour == White){
                    whiteKingSquare = squareIDFromIndices(i, j);
                    break;
                }
            }
        }
    }
    State *fakeState = new State();
    *fakeState = *state;
    fakeState->_colour_to_move = Black;
    vector<Move> legalResultingMovesForBlack = getLegalMoves(fakeState, Queen, false);
    for (auto move: legalResultingMovesForBlack){
        if (move._destination_square == whiteKingSquare){
            _square_under_check = whiteKingSquare;
            state->_white_king_is_in_check = true;
            return true;
        }
    }
    return false;
}

bool ChessRules::blackKingIsInCheck(State *state){
    string blackKingSquare;
    for (int i = 0; i < state->_board.size(); i++){
        for (int j = 0; j < state->_board.at(i).size(); j++){
            if (state->_board.at(i).at(j) != nullptr){
                if (state->_board.at(i).at(j)->_type == King &&
                        state->_board.at(i).at(j)->_colour == Black){
                    blackKingSquare = squareIDFromIndices(i, j);
                    break;
                }
            }
        }
    }
    State *fakeState = new State();
    *fakeState = *state;
    fakeState->_colour_to_move = White;
    vector<Move> legalResultingMovesForWhite = getLegalMoves(fakeState, Queen, false);
    for (auto move: legalResultingMovesForWhite){
        if (move._destination_square == blackKingSquare){
            _square_under_check = blackKingSquare;
            state->_black_king_is_in_check = true;
            return true;
        }
    }
    return false;
}

bool ChessRules::isInsufficientMaterial(State *state){
    int whiteMinorPieceCount = 0;
    int blackMinorPieceCount = 0;
    vector<pair<Piece, Colour>> minorPieces; //second element denotes colour of square
    for (int i = 0; i < state->_board.size(); i++){
        for (int j = 0; j < state->_board.at(i).size(); j++){
            if (state->_board.at(i).at(j) != nullptr){
                //Not insufficient material if pawn, queen, or rook is still in play
                if (state->_board.at(i).at(j)->_type == Queen ||
                        state->_board.at(i).at(j)->_type == Rook ||
                        state->_board.at(i).at(j)->_type == Pawn)
                    return false;
                else if (state->_board.at(i).at(j)->_type == Bishop || state->_board.at(i).at(j)->_type == Knight){
                    minorPieces.push_back(make_pair<Piece, Colour>(
                                              Piece(state->_board.at(i).at(j)->_colour, state->_board.at(i).at(j)->_type),
                                              i+j % 2 == 0 ? Black : White));
                    if (state->_board.at(i).at(j)->_colour == White)
                        whiteMinorPieceCount += 1;
                    if (state->_board.at(i).at(j)->_colour == Black)
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

int ChessRules::numberOfTimesThisStateSeen(string fen, map<string, int> *stateSeenCount){
    string cutFen;
    string delimiter = " ";
    size_t pos = 0;
    string subString;
    int i = 0;
    while ((pos = fen.find(delimiter)) != string::npos) {
        subString = fen.substr(0, pos);
        cutFen += subString;
        cutFen += " ";
        fen.erase(0, pos + delimiter.length());
        i++;
        if (i > 2)
            break;
    }
    if (stateSeenCount->count(cutFen) > 0)
        stateSeenCount->find(cutFen)->second += 1;
    else
        stateSeenCount->insert(pair<string, int>(cutFen, 1));
    //qDebug() << "state seen " << stateSeenCount->find(cutFen)->second << " times";
    return stateSeenCount->find(cutFen)->second;
}
