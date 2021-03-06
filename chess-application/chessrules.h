#ifndef CHESSRULES_H
#define CHESSRULES_H

#include <chessstructs.h>
#include <utility>
#include <QDebug>
#include "utils.h"
#include "bitboardutils.h"
#include <QElapsedTimer>
#include "zobristhasher.h"

class ChessRules: public BitBoardUtils
{
public:
    ChessRules();

    bool isInsufficientMaterial(State *state);
    State *getResultingStateFromMove(State *currentState, Move moveToMake);
    void updateCastlingInfo(Move move, State *state);

    float _accumulated_test_time;

    vector<Move> getLegalBitBoardMoves(State *state);
    void updateBitBoardWithMove(State *currentState, State *resultingState, Move move);
    bool bitBoardSquareIsUnderAttack(int index, BitBoard board, Colour colourAttacking);
    vector<Move> getBitBoardPseudoMovesForKing(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> getBitBoardPseudoMovesForPawn(int index, BitBoard board, Colour colourToMove, int numberOfMoves, ULL enPassantSquare);
    vector<Move> getBitBoardPseudoMovesForKnight(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> getBitBoardPseudoMovesForBishop(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> getBitBoardPseudoMovesForRook(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> getBitBoardPseudoMovesForQueen(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    bool bitBoardMoveCausedSelfCheck(Move move, BitBoard board);
    bool bitBoardWhiteKingIsInCheck(BitBoard board);
    bool bitBoardBlackKingIsInCheck(BitBoard board);
    ULL getBitBoardOfPossibleAttacksForBishop(int index, ULL occupancy);
    ULL getBitBoardOfPossibleAttacksForRook(int index, ULL occupancy);
    vector<Move> getBitBoardCastlingMoves(BitBoard board, CastlingInfo castlingInfo, Colour colourToMove, int numberOfMoves);

    ZobristHasher _hasher = ZobristHasher();

    int bitBoardNumberOfTimesThisStateSeen(ULL hash, map<ULL, int> *stateSeenCount);

    State* stateFromFEN(string fen);
    void runPERFTTest(State *state, int maxDepth, bool printDivide = false);
    void expandPERFTTree(State *currentState, map<int, int> *movePerDepthCounter, int currentDepth,
                         int maxDepth, bool printDivide, map<string, int> *moveTypeCounter, map<string, int> *divideMap, string divideString = "");
private:
    vector<pair<int, int>> _possible_knight_moves{make_pair(1,2), make_pair(1,-2), make_pair(2,1), make_pair(2,-1),
                make_pair(-2,1), make_pair(-2,-1), make_pair(-1,2), make_pair(-1,-2)};
    vector<pair<int, int>> _possible_king_moves{make_pair(0,1), make_pair(1,1), make_pair(1,0), make_pair(1,-1),
                make_pair(0,-1), make_pair(-1,-1), make_pair(-1,0), make_pair(-1,1)};


};

#endif // CHESSRULES_H
