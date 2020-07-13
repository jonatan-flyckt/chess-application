#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <chessstructs.h>
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
private:

    bool _is_game_over;
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
