#include "chessgame.h"

ChessGame::ChessGame(bool _user_is_white, string date, Difficulty difficulty, string name){
    _date = date;
    _user_colour = _user_is_white ? White : Black;
    _difficulty = difficulty;
    _player_name = name;
    _is_game_over = false;
    _is_draw = false;
    _white_won = false;
    _black_won = false;
    _white_resigned = false;
    _black_resigned = false;
    _current_state = gameStartingState();
    _state_vector = new vector<State*>();
    _state_vector->push_back(_current_state);
    updatePGN();
    _state_seen_count = new map<string, int>();
    _current_state->_legal_moves_from_state = _rules.getLegalMoves(_current_state, Queen);
    updatePGN();
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
    if (_white_resigned){
        _current_state->_is_game_over = true;
        _is_game_over = true;
        _black_won = true;
        return true;
    }
    else if (_black_resigned){
        _current_state->_is_game_over = true;
        _is_game_over = true;
        _white_won = true;
        return true;
    }
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
    resultingState->_moves_without_capture_or_pawn_advancement = (moveToMake._move_type == Capture || moveToMake._piece._type == Pawn) ? 0 : _current_state->_moves_without_capture_or_pawn_advancement+1;
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
    if (moveToMake._move_type == EnPassant)
        performEnPassantMove(moveToMake, resultingState);

    resultingState->_legal_moves_from_state = _rules.getLegalMoves(resultingState, _piece_selected_from_promotion);
    _state_vector->push_back(resultingState);
    _current_state->_next_state = resultingState;
    _current_state = resultingState;
    _current_state->_white_king_is_in_check = _rules.whiteKingIsInCheck(_current_state);
    _current_state->_black_king_is_in_check = _rules.blackKingIsInCheck(_current_state);
    if (_current_state->_white_king_is_in_check || _current_state->_black_king_is_in_check)
        _current_state->_square_under_check = _rules._square_under_check;
    _square_under_check = _rules._square_under_check;

    setFenForState(_current_state);
    if (_current_state->_legal_moves_from_state.size() == 0){ //End the game if there are no legal moves
        _current_state->_is_game_over = true;

        if (_current_state->_colour_to_move == Black && _current_state->_black_king_is_in_check){
            _white_won = true;
            _game_over_reason = "Check Mate";
        }
        else if (_current_state->_colour_to_move == White && _current_state->_white_king_is_in_check){
            _black_won = true;
            _game_over_reason = "Check Mate";
        }
        else{
            _is_draw = true;
            _game_over_reason = "Stalemate";
        }
    }
    if (_rules.isInsufficientMaterial(_current_state)){
        _current_state->_is_game_over = true;
        _is_draw = true;
        _game_over_reason = "Insufficient mating material";
    }
    if (_current_state->_moves_without_capture_or_pawn_advancement >= 100){
        _current_state->_is_game_over = true;
        _is_draw = true;
        _game_over_reason = "50 move rule";
    }
    if (moveToMake._piece._type == Pawn)
        _state_seen_count->clear();
    if (_rules.numberOfTimesThisStateSeen(_current_state->_fen_notation, _state_seen_count) >= 3){
        _current_state->_is_game_over = true;
        _is_draw = true;
        _game_over_reason = "Threefold repetition";
    }

    _is_game_over = _current_state->_is_game_over;
    _current_state->_move_to_state._algebraic_notation = algebraicNotationForMove(_current_state);
    updatePGN();


    qDebug() << QString::fromStdString(_portable_game_notation);
    return true;
}

void ChessGame::performEnPassantMove(Move move, State *state){
    int rowFrom = IndicesFromSquareID(move._origin_square).first;
    int colTo = IndicesFromSquareID(move._destination_square).second;
    state->_board.at(rowFrom).at(colTo) = nullptr;
}

State *ChessGame::getCurrent_state() const{
    return _current_state;
}

void ChessGame::setCurrent_state(State *current_state){
    _current_state = current_state;
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
    startingState->_moves_without_capture_or_pawn_advancement = 0;
    initiatePieces(startingState);
    setFenForState(startingState);
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
    string playerString = "";
    if (_player_name == "")
        playerString = "Player, Unknown";
    else
        playerString = "Player, " + _player_name;
    _portable_game_notation = "";
    _portable_game_notation += "[Event \"?\"]\n";
    _portable_game_notation += "[Site \"?\"]\n";
    _portable_game_notation += "[Date \"";
    _portable_game_notation += _date;
    _portable_game_notation += "\"]\n";
    _portable_game_notation += "[Round \"?\"]\n";
    _portable_game_notation += "[White \"";
    _portable_game_notation += _user_colour == White ? playerString : "Escape Chess Engine, " + stringFromDifficulty(_difficulty);
    _portable_game_notation += "\"]\n";
    _portable_game_notation += "[Black \"";
    _portable_game_notation += _user_colour == Black ? playerString : "Escape Chess Engine, " + stringFromDifficulty(_difficulty);
    _portable_game_notation += "\"]\n";
    _portable_game_notation += "[Result \"";
    if (!_is_game_over)
        _portable_game_notation += "*";
    else{
        if (_white_won)
            _portable_game_notation += "1-0";
        else if(_black_won)
            _portable_game_notation += "0-1";
        else
            _portable_game_notation += "1/2-1/2";
    }
    _portable_game_notation += "\"]\n\n";
    for (auto state: *_state_vector){
        if (state->_number_of_moves % 2 == 1){
            if (state->_number_of_moves > 0)
                _portable_game_notation += " ";
            _portable_game_notation += to_string((state->_number_of_moves+1)/2);
            _portable_game_notation += ".";
        }
        if (state->_number_of_moves > 0){
            _portable_game_notation += " ";
            if (state->_move_to_state._move_type == LongCastle){
                _portable_game_notation += "O-O-O";
                if (state->_white_king_is_in_check || state->_black_king_is_in_check)
                    _portable_game_notation += (state->_is_game_over) ? "#" : "+";
            }
            else if (state->_move_to_state._move_type == ShortCastle){
                _portable_game_notation += "O-O";
                if (state->_white_king_is_in_check || state->_black_king_is_in_check)
                    _portable_game_notation += (state->_is_game_over) ? "#" : "+";
            }
            else if (state->_move_to_state._move_type == Promotion || state->_move_to_state._move_type == PromotionCapture){
                if (state->_white_king_is_in_check || state->_black_king_is_in_check){
                    char capturePiece = state->_move_to_state._algebraic_notation[state->_move_to_state._algebraic_notation.size()-2];
                    string notation = state->_move_to_state._algebraic_notation.substr(0, state->_move_to_state._algebraic_notation.size()-2);
                    _portable_game_notation += notation + "=" + capturePiece;
                    _portable_game_notation += (state->_is_game_over) ? "#" : "+";
                }
                else{
                    char capturePiece = state->_move_to_state._algebraic_notation[state->_move_to_state._algebraic_notation.size()-1];
                    string notation = state->_move_to_state._algebraic_notation.substr(0, state->_move_to_state._algebraic_notation.size()-1);
                    _portable_game_notation += notation + "=" + capturePiece;
                }
            }
            else if (state->_move_to_state._move_type == EnPassant){
                _portable_game_notation += state->_move_to_state._algebraic_notation.substr(0, state->_move_to_state._algebraic_notation.size()-4);
            }
            else
                _portable_game_notation += state->_move_to_state._algebraic_notation;
        }
    }
    if (_is_game_over){
        if (_white_won)
            _portable_game_notation += "\n\n1-0";
        else if(_black_won)
            _portable_game_notation += "\n\n0-1";
        else
            _portable_game_notation += "\n\n1/2-1/2";
    }
}

void ChessGame::setFenForState(State *state){
    string fenBuilder = "";
    for (int i = state->_board.size()-1; i >= 0; i--){
        int emptySquareCounter = 0;
        bool pieceFoundOnLastSquare = false;
        for (int j = 0; j < state->_board.size(); j++){
            if (state->_board.at(i).at(j) == nullptr){
                emptySquareCounter++;
            }
            else{
                if (j == 7)
                   pieceFoundOnLastSquare = true;
                if (emptySquareCounter > 0){
                    fenBuilder += to_string(emptySquareCounter);
                }
                emptySquareCounter = 0;
                if (state->_board.at(i).at(j)->_type == Pawn)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'P' : 'p');
                else if (state->_board.at(i).at(j)->_type == Rook)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'R' : 'r');
                else if (state->_board.at(i).at(j)->_type == Bishop)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'B' : 'b');
                else if (state->_board.at(i).at(j)->_type == Knight)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'N' : 'n');
                else if (state->_board.at(i).at(j)->_type == Queen)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'Q' : 'q');
                else if (state->_board.at(i).at(j)->_type == King)
                    fenBuilder.push_back(state->_board.at(i).at(j)->_colour == White ? 'K' : 'k');
            }
        }
        if (!pieceFoundOnLastSquare)
            fenBuilder += to_string(emptySquareCounter);
        if (i > 0)
            fenBuilder.push_back('/');
    }
    fenBuilder.push_back(' ');
    fenBuilder.push_back(state->_colour_to_move == White ? 'w' : 'b');
    fenBuilder.push_back(' ');
    if ((state->_castling_info._white_castled || state->_castling_info._white_king_has_moved) &&
            (state->_castling_info._black_castled || state->_castling_info._black_king_has_moved)){
        fenBuilder.push_back('-');
    }
    else{
        if (!state->_castling_info._white_king_has_moved && !state->_castling_info._white_short_rook_has_moved)
            fenBuilder.push_back('K');
        if (!state->_castling_info._white_king_has_moved && !state->_castling_info._white_long_rook_has_moved)
            fenBuilder.push_back('Q');
        if (!state->_castling_info._black_king_has_moved && !state->_castling_info._black_short_rook_has_moved)
            fenBuilder.push_back('k');
        if (!state->_castling_info._black_king_has_moved && !state->_castling_info._black_long_rook_has_moved)
            fenBuilder.push_back('q');
    }
    fenBuilder.push_back(' ');
    for (auto strChar: enPassantTargetSquareForFEN(state->_move_to_state))
        fenBuilder.push_back(strChar);
    fenBuilder.push_back(' ');
    fenBuilder += to_string(state->_moves_without_capture_or_pawn_advancement);
    fenBuilder.push_back(' ');
    fenBuilder += to_string(int((state->_number_of_moves+2) / 2));
    state->_fen_notation = fenBuilder;
}

string ChessGame::enPassantTargetSquareForFEN(Move move){
    if (move._piece._type != Pawn)
        return "-";
    int rowFrom = IndicesFromSquareID(move._origin_square).first;
    int colFrom = IndicesFromSquareID(move._origin_square).second;
    int rowTo = IndicesFromSquareID(move._destination_square).first;
    int colTo = IndicesFromSquareID(move._destination_square).second;

    if (abs(rowFrom - rowTo) < 2)
        return "-";
    else{
        if (move._colour_performing_move == White)
            return squareIDFromIndices(rowTo-1, colTo);
        else
            return squareIDFromIndices(rowTo+1, colTo);
    }
}

string ChessGame::algebraicNotationForMove(State *state){
    string notation = "";
    Move move = state->_move_to_state;
    if (move._move_type == LongCastle){
        if (state->_white_king_is_in_check || state->_black_king_is_in_check){
            string str = "0-0-0";
            str += (_is_game_over) ? "#" : "+";
            return str;
        }
        else
            return "0-0-0";
    }
    else if (move._move_type == ShortCastle){
        if (state->_white_king_is_in_check || state->_black_king_is_in_check){
            string str = "0-0";
            str += (_is_game_over) ? "#" : "+";
            return str;
        }
        else
            return "0-0";
    }

    if (move._piece._type == Rook)
        notation += "R";
    else if (move._piece._type == Bishop)
        notation += "B";
    else if (move._piece._type == Knight)
        notation += "N";
    else if (move._piece._type == Queen)
        notation += "Q";
    else if (move._piece._type == King)
        notation += "K";

    State *previousState = state->_previous_state;
    vector<string> possibleOriginSquares;
    for (auto legalMove: previousState->_legal_moves_from_state){
        int i = IndicesFromSquareID(legalMove._origin_square).first;
        int j = IndicesFromSquareID(legalMove._origin_square).second;
        if (previousState->_board.at(i).at(j)->_type == move._piece._type && legalMove._destination_square == move._destination_square)
            possibleOriginSquares.push_back(legalMove._origin_square);
    }
    for (auto p: possibleOriginSquares)
        qDebug() << QString::fromStdString(p);
    if (possibleOriginSquares.size() > 1 && move._move_type != EnPassant){ //Need to indicate which piece made the move
        char file = move._origin_square[0];
        bool multipleOnSameFile = false;
        int count = 0;
        for (auto square: possibleOriginSquares){
            if (square[0] == file)
                count++;
        }
        if (count > 1)
            multipleOnSameFile = true;
        char rank = move._origin_square[1];
        bool multipleOnSameRank = false;
        count = 0;
        for (auto square: possibleOriginSquares){
            if (square[1] == rank)
                count++;
        }
        if (count > 1)
            multipleOnSameRank = true;
        if (multipleOnSameFile && multipleOnSameRank)
            notation += move._origin_square;
        else if (multipleOnSameRank)
            notation += file;
        else if (multipleOnSameFile)
            notation += rank;
        else
            notation += file;
    }
    if (move._move_type == EnPassant || (move._move_type == Capture && move._piece._type == Pawn) || move._move_type == PromotionCapture)
        notation += move._origin_square[0];

    if (move._move_type == Capture || move._move_type == EnPassant || move._move_type == PromotionCapture)
        notation += "x";

    notation += move._destination_square;

    if (move._move_type == EnPassant)
        notation += "e.p.";

    if (move._move_type == Promotion || move._move_type == PromotionCapture){
        int i = IndicesFromSquareID(move._destination_square).first;
        int j = IndicesFromSquareID(move._destination_square).second;
        if (state->_board.at(i).at(j)->_type == Queen)
            notation += "Q";
        else if (state->_board.at(i).at(j)->_type == Rook)
            notation += "R";
        else if (state->_board.at(i).at(j)->_type == Bishop)
            notation += "B";
        else if (state->_board.at(i).at(j)->_type == Knight)
            notation += "N";
    }

    if (state->_white_king_is_in_check || state->_black_king_is_in_check){
        if (state->_is_game_over)
            notation += "#";
        else
            notation += "+";
    }

    return notation;
}

Colour ChessGame::getUser_colour() const
{
    return _user_colour;
}

vector<State *> *ChessGame::getState_vector() const
{
    return _state_vector;
}

string ChessGame::getDate() const{
    return _date;
}

Difficulty ChessGame::getDifficulty() const{
    return _difficulty;
}

string ChessGame::getPortable_game_notation() const{
    return _portable_game_notation;
}
