#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <structs.h>

using namespace std;



class ChessGame
{
public:
    ChessGame(bool _user_is_white, string difficulty = "standard");
    ~ChessGame();

    string fenFromState(State state);
    State* gameStartingState();
    void updatePGN();


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
};

#endif // CHESSGAME_H
