#ifndef CHESSRULES_H
#define CHESSRULES_H

#include <chessstructs.h>
#include <utility>
#include <QDebug>
#include "utils.h"
#include "bitboardutils.h"
#include "zobristhasher.h"
#include <chrono>
#include <thread>
#include "magicslidinggenerator.h"

uint64_t nanosecond_measurement();
uint64_t millisecond_measurement();

class ChessRules: public BitBoardUtils
{
public:
    ChessRules();

    bool isInsufficientMaterial(State *state);
    State *getResultingStateFromMove(State *currentState, Move moveToMake);
    void updateCastlingInfo(Move move, State *state);
    vector<Move> getLegalMoves(State *state);
    void updateBitBoardWithMove(State *currentState, State *resultingState, Move move);
    bool squareIsUnderAttack(int index, BitBoard board, Colour colourAttacking);
    vector<Move> getPseudoLegalMovesForKing(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> gerPseudoLegalMovesForPawn(int index, BitBoard board, Colour colourToMove, int numberOfMoves, ULL enPassantSquare);
    vector<Move> getPseudoLegalMovesForKnight(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> getPseudoLegalMovesForBishop(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> getPseudoLegalMovesForRook(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    vector<Move> getPseudoLegalMovesForQueen(int index, BitBoard board, Colour colourToMove, int numberOfMoves);
    bool pseudoLegalMoveCausedSelfCheck(Move move, BitBoard board);
    bool whiteKingIsInCheck(BitBoard board);
    bool blackKingIsInCheck(BitBoard board);
    ULL getBitBoardOfPossibleAttacksForBishop(int index, ULL occupancy);
    ULL getBitBoardOfPossibleAttacksForRook(int index, ULL occupancy);
    vector<Move> getCastlingMoves(BitBoard board, CastlingInfo castlingInfo, Colour colourToMove, int numberOfMoves);
    unordered_map<Piece, vector<int>> getIndicesOfBitsForPieceTypes(BitBoard board);

    ZobristHasher _hasher = ZobristHasher();
    MagicSlidingGenerator _slide_move_generator = MagicSlidingGenerator();

    int bitBoardNumberOfTimesThisStateSeen(ULL hash, unordered_map<ULL, int> *stateSeenCount);

    State* stateFromFEN(string fen);
    bool runPerftTest(State *state, int maxDepth, map<string, int> *correctDivide = nullptr);
    void expandPerftTree(State *currentState, map<int, int> *movePerDepthCounter, int currentDepth,
                         int maxDepth, map<string, int> *moveTypeCounter, map<string, int> *divideMap, string divideString = "");
    bool testMoveGenerationCorrectness(string testSize="small");
    bool pawnsOnAdjacentColumns(int indexFirst, int indexSecond);


    float _accumulated_update_bit_board_time;
    float _accumulated_get_legal_bit_board_moves_timer;
    float _accumulated_update_castling_time;
    float _accumulated_kings_in_check_time;
    float _accumulated_hash_time;
    float _inner_accumulated_state_generation_time;

    float _pawn_timer;
    float _knight_timer;
    float _bishop_timer;
    float _rook_timer;
    float _queen_timer;
    float _king_timer;
    float _castling_timer;
    float _self_check_timer;

    float _self_check_first_timer;
    float _self_check_second_timer;

    float _attack_pawn_timer;
    float _attack_knight_timer;
    float _attack_bishop_timer;
    float _attack_rook_timer;

private:
    vector<pair<int, int>> _possible_knight_moves{make_pair(1,2), make_pair(1,-2), make_pair(2,1), make_pair(2,-1),
                make_pair(-2,1), make_pair(-2,-1), make_pair(-1,2), make_pair(-1,-2)};
    vector<pair<int, int>> _possible_king_moves{make_pair(0,1), make_pair(1,1), make_pair(1,0), make_pair(1,-1),
                make_pair(0,-1), make_pair(-1,-1), make_pair(-1,0), make_pair(-1,1)};






};

#endif // CHESSRULES_H
