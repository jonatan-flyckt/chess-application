#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <chessstructs.h>
#include <chessrules.h>
#include <QDebug>

using namespace std;

class ChessGame
{
public:
    ChessGame(bool _user_is_white, string difficulty = "standard");
    ~ChessGame();

    string fenFromState(State state);
    State* gameStartingState();
    void updatePGN();
    string squareIDFromIndices(int row, int col);
    void initiatePieces(State *startingState);

    void setFenForState(State *state);
    void setBitBoardForState(State *state);
    void setAlgebraicNotationForMove(Move move);

    vector<Move> getLegalMovesForSquare(State *state, string square);

    bool makeMove(string originSquare, string destinationSquare);
    vector<Move> getLegalMovesForCurrentState();
private:

    ChessRules _rules;
    bool _is_game_over;
    string _game_over_reason;
    bool _is_draw;
    bool _white_won;
    bool _black_won;
    Colour _user_colour;
    State *_current_state;
    vector<State*> *_state_vector;
    string _difficulty;

    string _pgn;

    vector<string> colsFromIndex{"a", "b", "c", "d", "e", "f", "g", "h"};
};

#endif // CHESSGAME_H
