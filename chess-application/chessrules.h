#ifndef CHESSRULES_H
#define CHESSRULES_H

#include <chessstructs.h>
#include <utility>

class ChessRules
{
public:
    ChessRules();

    vector<Move> getLegalMoves(State *state, bool checkIfCheck = true);

    bool isWhiteCheck(State *state);

    bool isBlackCheck(State *state);

    bool isGameOver(State *state);

    bool isWhiteWin(State *state);

    bool isBlackWin(State *state);

    bool isDraw(State *state);

    string squareIDFromIndices(int row, int col);

    vector<Move> checkIfMovesCauseCheck(vector<Move> movesToCheck, vector<vector<Piece *> > board, Colour colourToMove);

    vector<Move> getMovesForRook(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForBishop(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForQueen(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForKnight(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForKing(State *state, vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type, CastlingInfo castlingInfo, bool checkIfCheck);
    vector<Move> getMovesForPawn(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type, Move previousMove);

    State* resultingStateFromMove(Move move);

    pair<int, int> IndicesFromSquareID(string square);
    bool squareIsUnderAttack(int row, int col, State *state);
private:
    vector<string> _cols_from_index{"a", "b", "c", "d", "e", "f", "g", "h"};
    vector<pair<int, int>> _possible_knight_moves{make_pair(1,2), make_pair(1,-2), make_pair(2,1), make_pair(2,-1),
                make_pair(-2,1), make_pair(-2,-1), make_pair(-1,2), make_pair(-1,-2)};
    vector<pair<int, int>> _possible_king_moves{make_pair(0,1), make_pair(1,1), make_pair(1,0), make_pair(1,-1),
                make_pair(0,-1), make_pair(-1,-1), make_pair(-1,0), make_pair(-1,1)};
};

#endif // CHESSRULES_H
