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

ULL ZobristHasher::generateHashForPosition(BitBoard board, CastlingInfo castlingInfo, Colour turnToMove, ULL enPassantSquare){
    ULL hash = 0ULL;

    vector<int> indices = getIndicesOfBitsInBoard(board._white_pawns);

    for (auto index: indices)
        hash ^= _piece_numbers.at(_white_pawn).at(index);
    indices = getIndicesOfBitsInBoard(board._white_rooks);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_white_rook).at(index);
    indices = getIndicesOfBitsInBoard(board._white_knights);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_white_knight).at(index);
    indices = getIndicesOfBitsInBoard(board._white_bishops);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_white_bishop).at(index);
    indices = getIndicesOfBitsInBoard(board._white_queens);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_white_queen).at(index);
    indices = getIndicesOfBitsInBoard(board._white_king);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_white_king).at(index);

    indices = getIndicesOfBitsInBoard(board._black_pawns);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_black_pawn).at(index);
    indices = getIndicesOfBitsInBoard(board._black_rooks);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_black_rook).at(index);
    indices = getIndicesOfBitsInBoard(board._black_knights);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_black_knight).at(index);
    indices = getIndicesOfBitsInBoard(board._black_bishops);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_black_bishop).at(index);
    indices = getIndicesOfBitsInBoard(board._black_queens);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_black_queen).at(index);
    indices = getIndicesOfBitsInBoard(board._black_king);
    for (auto index: indices)
        hash ^= _piece_numbers.at(_black_king).at(index);

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

    for (auto number: _en_passant_file_numbers)
        hash ^= number;
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
    pieceVector.at(_white_pawn) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_rook) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_knight) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_bishop) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_queen) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_white_king) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_pawn) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_rook) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_knight) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_bishop) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_queen) = numbersArr;
    numbersArr.clear();

    for (int i = 0; i < 64; i++)
        numbersArr.push_back(generateRandomNumber());
    pieceVector.at(_black_king) = numbersArr;
    numbersArr.clear();

    return pieceVector;
}

int ZobristHasher::popLeastSignificantBitFromBoard(ULL *board){
    ULL tempBoard = *board ^(*board - 1);
    unsigned int halfFold = (unsigned) ((tempBoard & 0xffffffff) ^ (tempBoard >> 32));
    *board &= (*board-1);
    return _least_significant_bit_table[(halfFold * 0x783a9b23) >> 26];
}

vector<int> ZobristHasher::getIndicesOfBitsInBoard(ULL board){
    vector<int> indices;
    while (board)
        indices.push_back(popLeastSignificantBitFromBoard(&board));
    return indices;
}

