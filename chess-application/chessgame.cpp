#include "chessgame.h"

ChessGame::ChessGame(bool _user_is_white, string difficulty){
    _user_colour = _user_is_white ? White : Black;
    _difficulty = difficulty;
    _is_game_over = false;
    _is_draw = false;
    _white_won = false;
    _black_won = false;
    _current_state = gameStartingState();
    _state_vector = new vector<State*>();
    _state_vector->push_back(_current_state);
    updatePGN();
}


ChessGame::~ChessGame(){

}

string ChessGame::fenFromState(State state){
    return state._fen_notation;
}

State* ChessGame::gameStartingState(){
    State *startingState = new State();
    startingState->_previous_state = nullptr;

    //TODO: initiate game
    return startingState;
}

void ChessGame::updatePGN(){

}
