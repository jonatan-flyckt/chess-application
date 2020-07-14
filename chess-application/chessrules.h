#ifndef CHESSRULES_H
#define CHESSRULES_H

#include <chessstructs.h>

class ChessRules
{
public:
    ChessRules();

    vector<Move> getLegalMoves(State *state);

    vector<Move> getLegalMovesForSquare(State *state, string square);

    bool isWhiteCheck(State *state);

    bool isBlackCheck(State *state);

    bool isGameOver(State *state);

    bool isWhiteWin(State *state);

    bool isBlackWin(State *state);

    bool isDraw(State *state);

    vector<Move> getLegalMovesForRook(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    string squareIDFromIndices(int row, int col);

    vector<Move> checkIfMovesCauseCheck(vector<Move> movesToCheck, vector<vector<Piece *> > board, Colour colourToMove);
    vector<Move> getLegalMovesForBishop(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
private:
    vector<string> colsFromIndex{"a", "b", "c", "d", "e", "f", "g", "h"};
};

#endif // CHESSRULES_H
