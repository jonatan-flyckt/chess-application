#include "chessengine.h"

ChessEngine::ChessEngine(){
}

ChessEngine::~ChessEngine(){
}

Move ChessEngine::selectMoveFromState(State *state, Colour engineColour){
    QMap<int, int> moveIndexEvalValueMap;
    addAllPromotionSelections(state);

    for (int i = 0; i < state->_legal_moves_from_state.size(); i++){
        Move move =  state->_legal_moves_from_state.at(i);
        State *resultingState = _rules.getResultingStateFromMove(state, move);
        int eval = simpleMaterialEvaluation(resultingState);
        moveIndexEvalValueMap.insert(i, eval);
    }
    int currentBestIndex = 0;
    int currentBestEval = -1000;
    QMapIterator<int, int> i(moveIndexEvalValueMap);
    while (i.hasNext()) {
        i.next();
        if (currentBestEval == -1000)
            currentBestEval = i.value();
        if (state->_colour_to_move == White){
            if (i.value() > currentBestEval)
                currentBestIndex = i.key();
        }
        else{
            if (i.value() < currentBestEval)
                currentBestIndex = i.key();
        }
    }

    return state->_legal_moves_from_state.at(currentBestIndex);

}

void ChessEngine::addAllPromotionSelections(State *state){
    vector<Move> promotionMoves;
    for (int i = 0; i < state->_legal_moves_from_state.size(); i++){
        Move move =  state->_legal_moves_from_state.at(i);
        if (move._move_type == Promotion || move._move_type == PromotionCapture){
            move._promotion_selection = Queen;
            Move knightPromotion = move;
            knightPromotion._promotion_selection = Knight;
            promotionMoves.push_back(knightPromotion);
            Move bishopPromotion = move;
            bishopPromotion._promotion_selection = Bishop;
            promotionMoves.push_back(bishopPromotion);
            Move rookPromotion = move;
            rookPromotion._promotion_selection = Rook;
            promotionMoves.push_back(rookPromotion);
        }
    }
    for (auto move: promotionMoves)
        state->_legal_moves_from_state.push_back(move);
}

Move ChessEngine::makeRandomMove(State *state){
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

int ChessEngine::simpleMaterialEvaluation(State *state){
    int whiteVal = 0;
    int blackVal = 0;

    for (int i = 0; i < state->_board.size(); i++){
        for (int j = 0; j < state->_board.at(i).size(); j++){
            Piece *piece = state->_board.at(i).at(j);
            if (piece != nullptr){
                if (piece->_type == Queen){
                    if (piece->_colour == White)
                        whiteVal += 9;
                    else
                        blackVal += 9;
                }
                else if (piece->_type == Queen){
                    if (piece->_colour == White)
                        whiteVal += 9;
                    else
                        blackVal += 9;
                }
                else if (piece->_type == Rook){
                    if (piece->_colour == White)
                        whiteVal += 5;
                    else
                        blackVal += 5;
                }
                else if (piece->_type == Bishop){
                    if (piece->_colour == White)
                        whiteVal += 3;
                    else
                        blackVal += 3;
                }
                else if (piece->_type == Knight){
                    if (piece->_colour == White)
                        whiteVal += 3;
                    else
                        blackVal += 3;
                }
                else if (piece->_type == Pawn){
                    if (piece->_colour == White)
                        whiteVal += 1;
                    else
                        blackVal += 1;
                }
            }
        }
    }

    return whiteVal - blackVal;
}








