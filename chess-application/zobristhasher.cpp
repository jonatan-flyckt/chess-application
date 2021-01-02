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
        cout << _piece_numbers[Piece(Black, Pawn)].at(i) << endl;
}

ULL ZobristHasher::generateHashForPosition(BitBoard board, CastlingInfo castlingInfo, Colour turnToMove, ULL enPassantSquare){

}

ULL ZobristHasher::generateRandomNumber(){
    ULL r = 0;
    std::mt19937_64 generator(RANDOM_SEED + _current_random_index);
    _current_random_index++;
    std::uniform_int_distribution<ULL> distribution(RAND_MIN, RAND_MAX);
    for (int i = 0; i < 5; ++i)
        r = (r << 15) | (distribution(generator) & 0x7FFF);
    return r & 0xFFFFFFFFFFFFFFFFULL;
}

map<Piece, vector<ULL> > ZobristHasher::generateRandomNumbersForPieces(){
    map<Piece, vector<ULL>> pieceMap;
    vector<ULL> numbersArr;
    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_white_pawn, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_white_rook, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_white_knight, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_white_bishop, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_white_queen, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_white_king, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_black_pawn, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_black_rook, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_black_knight, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_black_bishop, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_black_queen, numbersArr);
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceMap.emplace(_black_king, numbersArr);
    numbersArr.clear();

    return pieceMap;
}
