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
    ChessGame(bool _user_is_white, string date, string difficulty, string name);
    ~ChessGame();

    string fenFromState(State state);
    State* gameStartingState();
    void updatePGN();
    string squareIDFromIndices(int row, int col);
    void initiatePieces(State *startingState);

    void setFenForState(State *state);

    vector<Move> getLegalMovesForSquare(State *state, string square);

    bool makeMove(string originSquare, string destinationSquare);
    vector<Move> getLegalMovesForCurrentState();
    pair<int, int> IndicesFromSquareID(string square);
    void updateCastlingInfo(Move move, State *state);

    void setPiece_selected_from_promotion(const PieceType &piece_selected_from_promotion);

    void performCastlingMove(Move move, State *state);
    PieceType getPiece_selected_from_promotion() const;

    void performEnPassantMove(Move move, State *state);
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

    string enPassantTargetSquareForFEN(Move move);
    string algebraicNotationForMove(State *state);
    Colour getUser_colour() const;

    vector<State *> *getState_vector() const;

    string getDate() const;

    string getDifficulty() const;

    string getPortable_game_notation() const;

private:

    map<string, int> *_state_seen_count;
    ChessRules _rules;
    Colour _user_colour;
    State *_current_state;
    vector<State*> *_state_vector;
    string _difficulty;
    string _date;
    string _portable_game_notation;
    PieceType _piece_selected_from_promotion;
    string _pgn;
    string _player_name;
    vector<string> _cols_from_index{"a", "b", "c", "d", "e", "f", "g", "h"};
};

#endif // CHESSGAME_H
