#include "chessrules.h"

ChessRules::ChessRules(){

}

vector<Move> ChessRules::getLegalMoves(State *state){
    vector<Move> legalMoves;
    Colour colourToMove = state->_colour_to_move;
    int numberOfMoves = state->_number_of_moves;
    vector<vector<Piece*>> board = state->_board;
    for(int i = 0; i < board.size(); i++){
        for (int j = 0; j < board.at(i).size(); j++){
            if (board.at(i).at(j)->_colour == colourToMove){
                if (board.at(i).at(j)->_type == Rook){
                    vector<Move> rookMoves = getLegalMovesForRook(board, colourToMove, i, j, numberOfMoves, Rook);
                    legalMoves.insert(legalMoves.end(), rookMoves.begin(), rookMoves.end());
                }
                else if (board.at(i).at(j)->_type == Bishop){
                    vector<Move> bishopMoves = getLegalMovesForBishop(board, colourToMove, i, j, numberOfMoves, Bishop);
                    legalMoves.insert(legalMoves.end(), bishopMoves.begin(), bishopMoves.end());
                }
                else if (board.at(i).at(j)->_type == Queen){
                    vector<Move> queenMoves = getLegalMovesForQueen(board, colourToMove, i, j, numberOfMoves, Queen);
                    legalMoves.insert(legalMoves.end(), queenMoves.begin(), queenMoves.end());
                }
            }
        }
    }
    return legalMoves;
}

vector<Move> ChessRules::checkIfMovesCauseCheck(vector<Move> movesToCheck, vector<vector<Piece*>> board, Colour colourToMove){
    //TODO: implement function
    return movesToCheck;
}

vector<Move> ChessRules::getLegalMovesForQueen(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type){
    vector<Move> queenMoves;
    vector<Move> straightMoves;
    vector<Move> diagonalMoves;
    straightMoves = getLegalMovesForRook(board, colourToMove, row, col, previousMoveNumber, type);
    diagonalMoves = getLegalMovesForBishop(board, colourToMove, row, col, previousMoveNumber, type);
    queenMoves.insert(queenMoves.end(), straightMoves.begin(), straightMoves.end());
    queenMoves.insert(queenMoves.end(), diagonalMoves.begin(), diagonalMoves.end());
    return queenMoves;
}

vector<Move> ChessRules::getLegalMovesForBishop(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type){
    Colour opponentColour = colourToMove == White ? Black : White;
    vector<Move> bishopMoves;
    string originID = squareIDFromIndices(row, col);

    for (int i = row, j = col; i < 8 && j < 8; i++, j++){
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1);
            move._is_capture = true;
            bishopMoves.push_back(move);
            break;
        }
        else
            break;
    }

    for (int i = row, j = col; i < 8 && j >= 0; i++, j--){
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1);
            move._is_capture = true;
            bishopMoves.push_back(move);
            break;
        }
        else
            break;
    }

    for (int i = row, j = col; i >= 0 && j < 8; i--, j++){
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1);
            move._is_capture = true;
            bishopMoves.push_back(move);
            break;
        }
        else
            break;
    }

    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--){
        if (board.at(i).at(j) == nullptr){
            bishopMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, j), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, j), previousMoveNumber+1);
            move._is_capture = true;
            bishopMoves.push_back(move);
            break;
        }
        else
            break;
    }

    bishopMoves = checkIfMovesCauseCheck(bishopMoves, board, colourToMove);
    return bishopMoves;
}

vector<Move> ChessRules::getLegalMovesForRook(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type){
    Colour opponentColour = colourToMove == White ? Black : White;
    vector<Move> rookMoves;
    string originID = squareIDFromIndices(row, col);
    for (int i = row; i < 8; i++){
        if (board.at(i).at(col) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, col), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, col), previousMoveNumber+1);
            move._is_capture = true;
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }
    for (int i = row; i >= 0; i--){
        if (board.at(i).at(col) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(i, col), previousMoveNumber+1));
        }
        else if (board.at(i).at(col)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(i, col), previousMoveNumber+1);
            move._is_capture = true;
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }

    for (int i = col; i < 8; i++){
        if (board.at(row).at(i) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(row, i), previousMoveNumber+1));
        }
        else if (board.at(row).at(i)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(row, i), previousMoveNumber+1);
            move._is_capture = true;
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }
    for (int i = col; i >= 0; i--){
        if (board.at(row).at(i) == nullptr){
            rookMoves.push_back(Move(colourToMove, Piece(colourToMove, type), originID,
                                     squareIDFromIndices(row, i), previousMoveNumber+1));
        }
        else if (board.at(row).at(i)->_colour == opponentColour){
            Move move = Move(colourToMove, Piece(colourToMove, type), originID,
                             squareIDFromIndices(row, i), previousMoveNumber+1);
            move._is_capture = true;
            rookMoves.push_back(move);
            break;
        }
        else
            break;
    }
    rookMoves = checkIfMovesCauseCheck(rookMoves, board, colourToMove);
    return rookMoves;
}

vector<Move> ChessRules::getLegalMovesForSquare(State *state, string square){

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
    squareID.append(colsFromIndex.at(col));
    squareID.append(to_string(row+1));
    return squareID;
}
