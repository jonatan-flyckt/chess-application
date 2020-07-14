#include "chessrules.h"

ChessRules::ChessRules(){

}

vector<Move> ChessRules::getLegalMoves(State *state, bool checkIfCheck){
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
        legalMoves = checkIfMovesCauseCheck(legalMoves, board, colourToMove);
    return legalMoves;
}

vector<Move> ChessRules::checkIfMovesCauseCheck(vector<Move> movesToCheck, vector<vector<Piece*>> board, Colour colourToMove){
    //TODO: implement function
    return movesToCheck;
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
    vector<Move> threateningMoves = getLegalMoves(fakeState, false);
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

bool ChessRules::isWhiteCheck(State *state){

}

bool ChessRules::isBlackCheck(State *state){

}

bool ChessRules::isGameOver(State *state){

}

bool ChessRules::isWhiteWin(State *state){

}

bool ChessRules::isBlackWin(State *state){

}

bool ChessRules::isDraw(State *state){

}

string ChessRules::squareIDFromIndices(int row, int col){
    string squareID = "";
    squareID.append(_cols_from_index.at(col));
    squareID.append(to_string(row+1));
    return squareID;
}

pair<int, int> ChessRules::IndicesFromSquareID(string square){
    int row;
    int col;
    row = stoi(square.substr(1, 1))-1;
    for (int i = 0; i < _cols_from_index.size(); i++){
        if (_cols_from_index.at(i) == square.substr(0, 1)){
            col = i;
            break;
        }
    }
    return make_pair(row, col);
}
