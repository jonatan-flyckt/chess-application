#include "zobristhasher.h"

ZobristHasher::ZobristHasher(){
    _current_random_index = 0;
    _piece_numbers = generateRandomNumbersForPieces();
    _black_to_move_number = generateRandomNumber();
    for (int i = 0; i < 4; i++)
        _castling_rights_numbers.push_back(generateRandomNumber());
    for (int i = 0; i < 8; i++)
        _en_passant_file_numbers.push_back(generateRandomNumber());
}

ULL ZobristHasher::generateHashForPosition(unordered_map<Piece, vector<int>> indicesOfBitsInBoard, CastlingInfo castlingInfo, Colour turnToMove, vector<int> enPassantIndexVector){
    ULL hash = 0ULL;

    for (auto index: indicesOfBitsInBoard[Piece(White, Pawn)])
        hash ^= _piece_numbers.at(_white_pawn_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(White, Rook)])
        hash ^= _piece_numbers.at(_white_rook_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(White, Knight)])
        hash ^= _piece_numbers.at(_white_knight_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(White, Bishop)])
        hash ^= _piece_numbers.at(_white_bishop_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(White, Queen)])
        hash ^= _piece_numbers.at(_white_queen_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(White, King)])
        hash ^= _piece_numbers.at(_white_king_index).at(index);

    for (auto index: indicesOfBitsInBoard[Piece(Black, Pawn)])
        hash ^= _piece_numbers.at(_black_pawn_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(Black, Rook)])
        hash ^= _piece_numbers.at(_black_rook_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(Black, Knight)])
        hash ^= _piece_numbers.at(_black_knight_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(Black, Bishop)])
        hash ^= _piece_numbers.at(_black_bishop_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(Black, Queen)])
        hash ^= _piece_numbers.at(_black_queen_index).at(index);
    for (auto index: indicesOfBitsInBoard[Piece(Black, King)])
        hash ^= _piece_numbers.at(_black_king_index).at(index);

    if (turnToMove == Black)
        hash ^= _black_to_move_number;

    if (!castlingInfo._white_castled){
        if (!castlingInfo._white_king_has_moved){
            if (!castlingInfo._white_short_rook_has_moved)
                hash ^= _castling_rights_numbers.at(0);
            if (!castlingInfo._white_long_rook_has_moved)
                hash ^= _castling_rights_numbers.at(1);
        }
    }
    if (!castlingInfo._black_castled){
        if (!castlingInfo._black_king_has_moved){
            if (!castlingInfo._black_short_rook_has_moved)
                hash ^= _castling_rights_numbers.at(2);
            if (!castlingInfo._black_long_rook_has_moved)
                hash ^= _castling_rights_numbers.at(3);
        }
    }

    if (enPassantIndexVector.size() > 0){
        int enPassantFile = enPassantIndexVector.at(0) % 8;
        hash ^= _en_passant_file_numbers.at(enPassantFile);
    }

    return hash;
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

vector<vector<ULL> > ZobristHasher::generateRandomNumbersForPieces(){
    vector<vector<ULL>> pieceVector;
    vector<ULL> numbersArr;
    pieceVector.resize(12);

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_pawn_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_rook_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_knight_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_bishop_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_queen_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_king_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_pawn_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_rook_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_knight_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_bishop_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_queen_index) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_king_index) = numbersArr;
    numbersArr.clear();

    return pieceVector;
}

