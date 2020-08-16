#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <chessstructs.h>
#include <chessrules.h>
#include <QDebug>
#include "utils.h"

using namespace std;

class ChessGame
{
public:
    ChessGame(bool _user_is_white, string date, Difficulty difficulty, string name);
    ~ChessGame();

    string fenFromState(State state);
    State* gameStartingState();
    void updatePGN();
    void initiatePieces(State *startingState);

    bool makeMove(string originSquare, string destinationSquare);
    vector<Move> getLegalMovesForCurrentState();

    void setPiece_selected_from_promotion(const PieceType &piece_selected_from_promotion);

    PieceType getPiece_selected_from_promotion() const;

    State *getCurrent_state() const;
    void setCurrent_state(State *current_state);

    bool _is_game_over;
    string _game_over_reason;
    bool _is_draw;
    bool _white_won;
    bool _black_won;
    bool _white_resigned;
    bool _black_resigned;
    string _square_under_check;

    string algebraicNotationForMove(State *state);
    Colour getUser_colour() const;

    vector<State *> *getState_vector() const;

    string getDate() const;

    Difficulty getDifficulty() const;

    string getPortable_game_notation() const;

    ChessGame* clone();

private:

    ChessRules _rules;
    Colour _user_colour;
    State *_current_state;
    vector<State*> *_state_vector;
    Difficulty _difficulty;
    string _date;
    string _portable_game_notation;
    PieceType _piece_selected_from_promotion;
    string _pgn;
    string _player_name;
};

#endif // CHESSGAME_H
