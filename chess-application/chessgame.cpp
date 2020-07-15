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
    _current_state->_legal_moves_from_state = _rules.getLegalMoves(_current_state, Queen);
}

ChessGame::~ChessGame(){
    for (auto state: *_state_vector)
        delete  state;
    delete _state_vector;
}

vector<Move> ChessGame::getLegalMovesForCurrentState(){
    return _current_state->_legal_moves_from_state;
}

bool ChessGame::makeMove(string originSquare, string destinationSquare){
    Move moveToMake;
    bool moveWasLegal = false;
    for (auto move: _current_state->_legal_moves_from_state){
        if (move._origin_square == originSquare && move._destination_square == destinationSquare){
            moveToMake = move;
            moveWasLegal = true;
        }
    }
    if (!moveWasLegal)
        return false;

    _current_state->_move_from_state = moveToMake;

    int rowFrom = IndicesFromSquareID(originSquare).first;
    int colFrom = IndicesFromSquareID(originSquare).second;
    int rowTo = IndicesFromSquareID(destinationSquare).first;
    int colTo = IndicesFromSquareID(destinationSquare).second;

    Piece pieceToMove = moveToMake._piece;

    State *resultingState = new State();
    resultingState->_colour_to_move = _current_state->_colour_to_move == White ? Black : White;
    resultingState->_move_to_state = moveToMake;
    resultingState->_previous_state = _current_state;
    resultingState->_number_of_moves = _current_state->_number_of_moves+1;
    resultingState->_moves_without_capture = (moveToMake._move_type == Capture || moveToMake._move_type == EnPassant || moveToMake._move_type == PromotionCapture) ? _current_state->_moves_without_capture+1 : 0;
    resultingState->_castling_info = _current_state->_castling_info;

    for (int i = 0; i < _current_state->_board.size(); i++){
        for (int j = 0; j < _current_state->_board.at(i).size(); j++){
            resultingState->_board.at(i).at(j) = _current_state->_board.at(i).at(j);
        }
    }
    resultingState->_board.at(rowFrom).at(colFrom) = nullptr;
    resultingState->_board.at(rowTo).at(colTo) = new Piece(pieceToMove._colour, pieceToMove._type);

    //Handle castling info and moves
    updateCastlingInfo(moveToMake, resultingState);
    if (moveToMake._move_type == LongCastle || moveToMake._move_type == ShortCastle)
        performCastlingMove(moveToMake, resultingState);
    if (moveToMake._move_type == Promotion || moveToMake._move_type == PromotionCapture)
        resultingState->_board.at(rowTo).at(colTo) = new Piece(pieceToMove._colour, _piece_selected_from_promotion);
    if (moveToMake._move_type == Capture || moveToMake._move_type == PromotionCapture || moveToMake._move_type == EnPassant)
        resultingState->_moves_without_capture = 0;
    if (moveToMake._move_type == EnPassant)
        performEnPassantMove(moveToMake, resultingState);


    //TODO: 3 move repeating rule
    resultingState->_legal_moves_from_state = _rules.getLegalMoves(resultingState, _piece_selected_from_promotion);
    //TODO: Check if game is over or if check occurred
    _state_vector->push_back(resultingState);
    _current_state = resultingState;

    return true;
}

void ChessGame::performEnPassantMove(Move move, State *state){
    int rowFrom = IndicesFromSquareID(move._origin_square).first;
    int colTo = IndicesFromSquareID(move._destination_square).second;
    state->_board.at(rowFrom).at(colTo) == nullptr;
}

void ChessGame::performCastlingMove(Move move, State *state){
    if (move._move_type == LongCastle){
        if (move._colour_performing_move == White){
            state->_board.at(0).at(0) = nullptr;
            state->_board.at(0).at(4) = nullptr;
            state->_board.at(0).at(2) = new Piece(White, King);
            state->_board.at(0).at(3) = new Piece(White, Rook);
        }
        else{
            state->_board.at(7).at(0) = nullptr;
            state->_board.at(7).at(4) = nullptr;
            state->_board.at(7).at(2) = new Piece(Black, King);
            state->_board.at(7).at(3) = new Piece(Black, Rook);
        }
    }
    if (move._move_type == ShortCastle){
        if (move._colour_performing_move == White){
            state->_board.at(0).at(7) = nullptr;
            state->_board.at(0).at(4) = nullptr;
            state->_board.at(0).at(6) = new Piece(White, King);
            state->_board.at(0).at(5) = new Piece(White, Rook);
        }
        else{
            state->_board.at(7).at(7) = nullptr;
            state->_board.at(7).at(4) = nullptr;
            state->_board.at(7).at(6) = new Piece(Black, King);
            state->_board.at(7).at(5) = new Piece(Black, Rook);
        }
    }
}

PieceType ChessGame::getPiece_selected_from_promotion() const{
    return _piece_selected_from_promotion;
}

void ChessGame::updateCastlingInfo(Move move, State *state){
    if (move._piece._type == Rook){
        if (move._piece._colour == White){
            if (move._origin_square == "a1")
                state->_castling_info._white_long_rook_has_moved = true;
            else if (move._origin_square == "h1")
                state->_castling_info._white_short_rook_has_moved = true;
        }
        else{
            if (move._origin_square == "a8")
                state->_castling_info._black_long_rook_has_moved = true;
            else if (move._origin_square == "h8")
                state->_castling_info._black_short_rook_has_moved = true;
        }
    }
    else if (move._piece._type == King){
        if (move._piece._colour == White)
            state->_castling_info._white_king_has_moved = true;
        else
            state->_castling_info._black_king_has_moved = true;
    }
    if (move._move_type == LongCastle || move._move_type == ShortCastle){
        if (move._piece._colour == White){
            state->_castling_info._white_castled = true;
            state->_castling_info._white_king_has_moved = true;
        }
        else{
            state->_castling_info._black_castled = true;
            state->_castling_info._black_king_has_moved = true;
        }
    }
}

void ChessGame::setPiece_selected_from_promotion(const PieceType &piece_selected_from_promotion)
{
    _piece_selected_from_promotion = piece_selected_from_promotion;
}

string ChessGame::fenFromState(State state){
    return state._fen_notation;
}

State* ChessGame::gameStartingState(){
    State *startingState = new State;
    startingState->_previous_state = nullptr;
    startingState->_colour_to_move = White;
    startingState->_number_of_moves = 0;
    startingState->_previous_state = nullptr;
    startingState->_moves_without_capture = 0;
    initiatePieces(startingState);
    setFenForState(startingState);
    setBitBoardForState(startingState);
    return startingState;
}

void ChessGame::initiatePieces(State *startingState){
    for (int i = 0; i < startingState->_board.at(1).size(); i++)
        startingState->_board.at(1).at(i) = new Piece(White, Pawn);
    startingState->_board.at(0).at(0) = new Piece(White, Rook);
    startingState->_board.at(0).at(7) = new Piece(White, Rook);
    startingState->_board.at(0).at(1) = new Piece(White, Knight);
    startingState->_board.at(0).at(6) = new Piece(White, Knight);
    startingState->_board.at(0).at(2) = new Piece(White, Bishop);
    startingState->_board.at(0).at(5) = new Piece(White, Bishop);
    startingState->_board.at(0).at(3) = new Piece(White, Queen);
    startingState->_board.at(0).at(4) = new Piece(White, King);
    for (int i = 0; i < startingState->_board.at(6).size(); i++)
        startingState->_board.at(6).at(i) = new Piece(Black, Pawn);
    startingState->_board.at(7).at(0) = new Piece(Black, Rook);
    startingState->_board.at(7).at(7) = new Piece(Black, Rook);
    startingState->_board.at(7).at(1) = new Piece(Black, Knight);
    startingState->_board.at(7).at(6) = new Piece(Black, Knight);
    startingState->_board.at(7).at(2) = new Piece(Black, Bishop);
    startingState->_board.at(7).at(5) = new Piece(Black, Bishop);
    startingState->_board.at(7).at(3) = new Piece(Black, Queen);
    startingState->_board.at(7).at(4) = new Piece(Black, King);
}

void ChessGame::updatePGN(){

}

void ChessGame::setFenForState(State *state){

}

void ChessGame::setBitBoardForState(State *state){

}

void ChessGame::setAlgebraicNotationForMove(Move move){

}

vector<Move> ChessGame::getLegalMovesForSquare(State *state, string square){

}

string ChessGame::squareIDFromIndices(int row, int col){
    string squareID = "";
    squareID.append(_cols_from_index.at(col));
    squareID.append(to_string(row+1));
    return squareID;
}

pair<int, int> ChessGame::IndicesFromSquareID(string square){
    int row;
    int col;
    row = stoi(square.substr(1, 1))-1;
    for (int i = 0; i < _cols_from_index.size(); i++){
        if (_cols_from_index.at(i) == square.substr(0, 1)){
            col = i;
            break;
        }
    }
    return make_pair(row, col);
}
