#include "chessengine.h"

ChessEngine::ChessEngine(){

}

ChessEngine::~ChessEngine(){

}

Move ChessEngine::selectMoveFromState(State *state, Colour engineColour){



    //TEMP: random move
    vector<Move> in = state->_legal_moves_from_state;
    vector<Move> out;
    sample(in.begin(),
           in.end(),
           back_inserter(out),
           1,
           mt19937{random_device{}()});
    return out[0];
}

Move ChessEngine::miniMax(State *state, Colour engineColour){

}

int ChessEngine::evaluateState(State *state, Colour engineColour){

}
