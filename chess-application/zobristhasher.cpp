#include "zobristhasher.h"

ZobristHasher::ZobristHasher(){
    _current_random_index = 0;
    _piece_numbers = generateRandomNumbersForPieces();
    _black_to_move_number = generateRandomNumber();
    for (int i = 0; i < 4; i++)
        _castling_rights_numbers.push_back(generateRandomNumber());
    for (int i = 0; i < 8; i++)
        _en_passant_file_numbers.push_back(generateRandomNumber());

    for (int i = 0; i < 64; i++)
        cout << _piece_numbers[Piece(Black, Pawn)][i] << endl;
    int hej = 1;
}

ULL ZobristHasher::generateHashForPosition(BitBoard board, CastlingInfo castlingInfo, Colour turnToMove, ULL enPassantSquare){

}

ULL ZobristHasher::generateRandomNumber(){
    /*
    srand(RANDOM_SEED);
    int index = _current_random_index;
    while(index-- >0)
        rand();
    _current_random_index++;
    return rand();
    */

    thread_local std::mt19937 engine;  // or a different engine
    engine.seed(RANDOM_SEED + _current_random_index);

    _current_random_index++;
    return engine();


}

map<Piece, ULL *> ZobristHasher::generateRandomNumbersForPieces(){
    map<Piece, ULL*> pieceMap;
    ULL numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(White, Pawn), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(White, Rook), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(White, Knight), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(White, Bishop), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(White, Queen), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(White, King), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(Black, Pawn), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(Black, Rook), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(Black, Knight), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(Black, Bishop), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(Black, Queen), numbersArr);

    numbersArr[64];
    for (int i = 0; i < 64; i++)
        numbersArr[i] = generateRandomNumber();
    pieceMap.emplace(Piece(Black, King), numbersArr);

}
