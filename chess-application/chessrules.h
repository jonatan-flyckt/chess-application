#ifndef CHESSRULES_H
#define CHESSRULES_H

#include <chessstructs.h>

class ChessRules
{
public:
    ChessRules();

    vector<Move> getLegalMoves(State state);

    vector<Move> getLegalMovesForSquare(string square);

    bool isCheck(State);

    bool isGameOver(State);

    bool isWhiteWin(State);

    bool isBlackWin(State);

    bool isDraw(State);

};

#endif // CHESSRULES_H
