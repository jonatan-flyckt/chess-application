#ifndef CHESSRULES_H
#define CHESSRULES_H

#include <chessstructs.h>
#include <utility>
#include <QDebug>
#include "utils.h"
#include "bitboardutils.h"
#include <QElapsedTimer>

class ChessRules: public BitBoardUtils
{
public:
    ChessRules();

    vector<Move> getLegalMoves(State *state, PieceType promotionPiece = Queen, bool checkIfCheck = true);

    bool whiteKingIsInCheck(State *state);

    bool blackKingIsInCheck(State *state);

    vector<Move> checkIfMovesCauseCheckForSelf(vector<Move> movesToCheck, vector<vector<Piece *> > board, Colour colourToMove, State *state, PieceType promotionPiece);

    vector<Move> getMovesForRook(vector<vector<Piece*>> board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForBishop(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForQueen(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForKnight(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type);
    vector<Move> getMovesForKing(State *state, vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type, CastlingInfo castlingInfo, bool checkIfCheck);
    vector<Move> getMovesForPawn(vector<vector<Piece *> > board, Colour colourToMove, int row, int col, int previousMoveNumber, PieceType type, Move previousMove);

    vector<Move> getLegalBitBoardMoves(State *state);
    vector<Move> getBitBoardMovesForKnight(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    bool bitBoardSquareIsUnderAttack(int index);


    bool squareIsUnderAttack(int row, int col, State *state);
    void performCheckCheckEnPassantMove(Move move, State *state);
    void performCheckCheckCastlingMove(Move move, State *state);

    string _square_under_check;
    bool isInsufficientMaterial(State *state);
    int numberOfTimesThisStateSeen(string fen, map<string, int> *stateSeenCount);
    State *getResultingStateFromMove(State *currentState, Move moveToMake);
    void updateCastlingInfo(Move move, State *state);
    void performCastlingMove(Move move, State *state);
    void performEnPassantMove(Move move, State *state);
    void setFenForState(State *state);
    string enPassantTargetSquareForFEN(Move move);


    QElapsedTimer _test_timer;
    float _accumulated_test_time;

    vector<Move> getBitBoardMovesForPawn(int index, BitBoard board, Colour colourToMove, int numberOfMoves, ULL enPassantSquare);
private:
    vector<pair<int, int>> _possible_knight_moves{make_pair(1,2), make_pair(1,-2), make_pair(2,1), make_pair(2,-1),
                make_pair(-2,1), make_pair(-2,-1), make_pair(-1,2), make_pair(-1,-2)};
    vector<pair<int, int>> _possible_king_moves{make_pair(0,1), make_pair(1,1), make_pair(1,0), make_pair(1,-1),
                make_pair(0,-1), make_pair(-1,-1), make_pair(-1,0), make_pair(-1,1)};


};

#endif // CHESSRULES_H
