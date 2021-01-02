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
    _current_state->_state_seen_count = new map<string, int>();
    _current_state->_bit_board_state_seen_count = new map<ULL, int>();
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

    moveToMake._promotion_selection = _piece_selected_from_promotion;

    State *resultingState = _rules.getResultingStateFromMove(_current_state, moveToMake);
    _is_draw = resultingState->_is_draw;
    _white_won = resultingState->_white_won;
    _black_won = resultingState->_black_won;
    _game_over_reason = resultingState->_game_over_reason;
    _state_vector->push_back(resultingState);

    _current_state = resultingState;
    _is_game_over = _current_state->_is_game_over;
    _current_state->_move_to_state._algebraic_notation = algebraicNotationForMove(_current_state);
    updatePGN();

    //qDebug() << QString::fromStdString(_portable_game_notation);
    return true;
}

State *ChessGame::getCurrent_state() const{
    return _current_state;
}

void ChessGame::setCurrent_state(State *current_state){
    _current_state = current_state;
}

PieceType ChessGame::getPiece_selected_from_promotion() const{
    return _piece_selected_from_promotion;
}

void ChessGame::setPiece_selected_from_promotion(const PieceType &piece_selected_from_promotion){
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
    initiateBitBoard(startingState);

    vector<Move> moves = _rules.getLegalBitBoardMoves(startingState);

    _rules.setFenForState(startingState);
    return startingState;
}

void ChessGame::initiateBitBoard(State *startingState){
    startingState->_bit_board._white_pawns = _starting_bitboard[Piece(White, Pawn)];
    startingState->_bit_board._white_rooks = _starting_bitboard[Piece(White, Rook)];
    startingState->_bit_board._white_bishops = _starting_bitboard[Piece(White, Bishop)];
    startingState->_bit_board._white_knights = _starting_bitboard[Piece(White, Knight)];
    startingState->_bit_board._white_king = _starting_bitboard[Piece(White, King)];
    startingState->_bit_board._white_queens = _starting_bitboard[Piece(White, Queen)];

    startingState->_bit_board._black_pawns = _starting_bitboard[Piece(Black, Pawn)];
    startingState->_bit_board._black_rooks = _starting_bitboard[Piece(Black, Rook)];
    startingState->_bit_board._black_bishops = _starting_bitboard[Piece(Black, Bishop)];
    startingState->_bit_board._black_knights = _starting_bitboard[Piece(Black, Knight)];
    startingState->_bit_board._black_king = _starting_bitboard[Piece(Black, King)];
    startingState->_bit_board._black_queens = _starting_bitboard[Piece(Black, Queen)];

    startingState->_bit_board._all_white_pieces = startingState->_bit_board._white_pawns |
            startingState->_bit_board._white_rooks | startingState->_bit_board._white_bishops |startingState->_bit_board._white_knights |
            startingState->_bit_board._white_king |startingState->_bit_board._white_queens;
    startingState->_bit_board._all_black_pieces = startingState->_bit_board._black_pawns |
            startingState->_bit_board._black_rooks | startingState->_bit_board._black_bishops |startingState->_bit_board._black_knights |
            startingState->_bit_board._black_king |startingState->_bit_board._black_queens;
    startingState->_bit_board._all_pieces = startingState->_bit_board._all_white_pieces | startingState->_bit_board._all_black_pieces;

    //printBoard(startingState->_bit_board._all_white_pieces);
    //printBoard(startingState->_bit_board._all_black_pieces);
    //printBoard(startingState->_bit_board._all_pieces);
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
    //for (auto p: possibleOriginSquares)
    //    qDebug() << QString::fromStdString(p);
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

Colour ChessGame::getUser_colour() const{
    return _user_colour;
}

vector<State *> *ChessGame::getState_vector() const{
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

ChessGame *ChessGame::clone(){
    return new ChessGame(*this);
}
