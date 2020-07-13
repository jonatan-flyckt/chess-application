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
    for (auto state: *_state_vector)
        delete  state;
    delete _state_vector;
}

string ChessGame::fenFromState(State state){
    return state._fen_notation;
}

State* ChessGame::gameStartingState(){
    State *startingState = new State();
    startingState->_previous_state = nullptr;
    startingState->_colour_to_move = White;
    startingState->_white_castled = false;
    startingState->_black_castled = false;
    Move fakeStartingMove;
    fakeStartingMove._move_number = 0;
    startingState->_move_to_state = fakeStartingMove;
    startingState->_number_of_moves = 0;
    startingState->_previous_state = nullptr;
    startingState->_moves_without_capture = 0;
    initiatePieces(startingState);
    setFenForState(startingState);
    setBitBoardForState(startingState);
    setLegalMovesForState(startingState);
    return startingState;
}

void ChessGame::initiatePieces(State *startingState){
    for (int i = 0; i < startingState->_board.at(1).size(); i++)
        startingState->_board.at(1).at(i) = new pair<Colour, Piece>{White, Pawn};
    startingState->_board.at(0).at(0) = new pair<Colour, Piece>{White, Rook};
    startingState->_board.at(0).at(7) = new pair<Colour, Piece>{White, Rook};
    startingState->_board.at(0).at(1) = new pair<Colour, Piece>{White, Knight};
    startingState->_board.at(0).at(6) = new pair<Colour, Piece>{White, Knight};
    startingState->_board.at(0).at(2) = new pair<Colour, Piece>{White, Bishop};
    startingState->_board.at(0).at(5) = new pair<Colour, Piece>{White, Bishop};
    startingState->_board.at(0).at(3) = new pair<Colour, Piece>{White, Queen};
    startingState->_board.at(0).at(4) = new pair<Colour, Piece>{White, King};
    for (int i = 0; i < startingState->_board.at(6).size(); i++)
        startingState->_board.at(6).at(i) = new pair<Colour, Piece>{Black, Pawn};
    startingState->_board.at(7).at(0) = new pair<Colour, Piece>{Black, Rook};
    startingState->_board.at(7).at(7) = new pair<Colour, Piece>{Black, Rook};
    startingState->_board.at(7).at(1) = new pair<Colour, Piece>{Black, Knight};
    startingState->_board.at(7).at(6) = new pair<Colour, Piece>{Black, Knight};
    startingState->_board.at(7).at(2) = new pair<Colour, Piece>{Black, Bishop};
    startingState->_board.at(7).at(5) = new pair<Colour, Piece>{Black, Bishop};
    startingState->_board.at(7).at(3) = new pair<Colour, Piece>{Black, Queen};
    startingState->_board.at(7).at(4) = new pair<Colour, Piece>{Black, King};
}

void ChessGame::updatePGN(){

}

void ChessGame::setFenForState(State *state){

}

void ChessGame::setBitBoardForState(State *state){

}

void ChessGame::setLegalMovesForState(State *state){

}

string ChessGame::squareIDFromIndices(int row, int col){
    string squareID = "";
    squareID.append(colsFromIndex.at(col));
    squareID.append(to_string(row+1));
    return squareID;
}
