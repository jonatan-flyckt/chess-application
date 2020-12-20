#include "bitboardutils.h"

ULL* BitBoardUtils::generateKnightMoveSet(){
    ULL* attackMap = (ULL*)malloc(64 * sizeof (ULL));
    vector<int> knightShifts = {6, 10, 15, 17};

    for (int square = 0; square < 64; square++){
        ULL oneULL = 1;
        ULL squareBit = oneULL << square;
        vector<ULL> moveList;
        for (auto shift: knightShifts){ //Shift in all 8 possible knight directions
            moveList.push_back(squareBit << shift);
            moveList.push_back(squareBit >> shift);
        }

        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        //Remove moves that ended up on the other side if knight is on A,B,G, or H
        if (square % 8 == 6 || square % 8 == 7)
            moveBoard &= 0xfcfcfcfcfcfcfcfc;
        else if (square % 8 == 0 || square % 8 == 1)
            moveBoard &= 0x3f3f3f3f3f3f3f3f;

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap[square] = moveBoard;
    }

    return attackMap;
}

ULL* BitBoardUtils::generateKingMoveSet(){
    ULL* attackMap = (ULL*)malloc(64 * sizeof (ULL));
    vector<int> kingShifts = {1, 7, 8, 9};

    for (int square = 0; square < 64; square++){
        ULL oneULL = 1;
        ULL squareBit = oneULL << square;
        vector<ULL> moveList;
        for (auto shift: kingShifts){
            moveList.push_back(squareBit << shift);
            moveList.push_back(squareBit >> shift);
        }

        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        if (square % 8 == 7)
            moveBoard &= 0xfcfcfcfcfcfcfcfc;
        else if (square % 8 == 0)
            moveBoard &= 0x3f3f3f3f3f3f3f3f;

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap[square] = moveBoard;
    }
    return attackMap;
}

ULL* BitBoardUtils::generateRookMoveSet(){
    ULL* attackMap = (ULL*)malloc(64 * sizeof (ULL));

    for (int square = 0; square < 64; square++){
        ULL oneULL = 1;
        ULL squareBit = oneULL << square;
        vector<ULL> moveList;
        for (int i = 8; i < 64; i += 8){
            moveList.push_back(squareBit << i);
            moveList.push_back(squareBit >> i);
        }

        for (int j = 1; j < 8; j++){
            if (square % 8 + j < 8)
                moveList.push_back(squareBit << j);
            if (square % 8 - j >= 0)
                moveList.push_back(squareBit >> j);
        }


        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap[square] = moveBoard;
    }
    return attackMap;

}





ULL* BitBoardUtils::generateQueenMoveSet(){
    ULL* attackMap = (ULL*)malloc(64 * sizeof (ULL));

    for (int square = 0; square < 64; square++){
        vector<ULL> moveList;

        moveList.push_back(_bishop_move_set[square]);
        moveList.push_back(_rook_move_set[square]);

        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap[square] = moveBoard;
    }
    return attackMap;
}

ULL* BitBoardUtils::generatePawnMoveSet(Colour colour){
    ULL* attackMap = (ULL*)malloc(64 * sizeof (ULL));
    for (int square = 0; square < 64; square++){
        vector<ULL> moveList;

        if (square > 7 && square < 56){
            ULL oneULL = 1;
            ULL squareBit = oneULL << square;

            moveList.push_back(colour == White ? squareBit << 8 : squareBit >> 8);
            if (square < 16 || square > 47)
                moveList.push_back(colour == White ? squareBit << 16 : squareBit >> 16);
        }

        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap[square] = moveBoard;
    }
    return attackMap;
}

ULL* BitBoardUtils::generatePawnCaptureSet(Colour colour){
    ULL* attackMap = (ULL*)malloc(64 * sizeof (ULL));
    for (int square = 0; square < 64; square++){
        vector<ULL> moveList;

        if (square > 7 && square < 56){
            ULL oneULL = 1;
            ULL squareBit = oneULL << square;

            if (square % 8 != 0)
                moveList.push_back(colour == White ? squareBit << 7 : squareBit >> 9);
            if (square % 8 != 7)
                moveList.push_back(colour == White ? squareBit << 9 : squareBit >> 7);
        }

        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap[square] = moveBoard;
    }
    return attackMap;
}

map<Piece, ULL> BitBoardUtils::generateStartingPosition(){
    map<Piece, ULL> startingBoard;

    ULL board = 0;
    for (int i = 8; i < 16; i++){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(White, Pawn), board);

    board = 0;
    for (auto i: vector{0, 7}){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(White, Rook), board);

    board = 0;
    for (auto i: vector{1, 6}){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(White, Knight), board);

    board = 0;
    for (auto i: vector{2, 5}){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(White, Bishop), board);

    board = 0;
    board |= 1ULL << 3;
    startingBoard.insert_or_assign(Piece(White, Queen), board);

    board = 0;
    board |= 1ULL << 4;
    startingBoard.insert_or_assign(Piece(White, King), board);


    board = 0;
    for (int i = 48; i < 56; i++){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(Black, Pawn), board);

    board = 0;
    for (auto i: vector{56, 63}){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(Black, Rook), board);

    board = 0;
    for (auto i: vector{57, 62}){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(Black, Knight), board);

    board = 0;
    for (auto i: vector{58, 61}){
        ULL squareBit = 1ULL << i;
        board |= squareBit;
    }
    startingBoard.insert_or_assign(Piece(Black, Bishop), board);

    board = 0;
    board |= 1ULL << 59;
    startingBoard.insert_or_assign(Piece(Black, Queen), board);

    board = 0;
    board |= 1ULL << 60;
    startingBoard.insert_or_assign(Piece(Black, King), board);

    return startingBoard;
}

pair<ULL*, ULL*> BitBoardUtils::generateBitMasks(){
    ULL* trueMask = (ULL*)malloc(64 * sizeof (ULL));
    ULL* falseMask = (ULL*)malloc(64 * sizeof (ULL));
    for (int i = 0; i < 64; i++){
        trueMask[i] = 0ULL | (1ULL << i);
        falseMask[i] = ~trueMask[i];
        //printBoard(trueMask[i]);
        //printBoard(falseMask[i]);
    }
    return pair(trueMask, falseMask);
}

int BitBoardUtils::countBitsInBoard(ULL board){
    int count;
    for (count = 0; board; count++, board &= board - 1);
    return count;
}

int BitBoardUtils::popLeastSignificantBitFromBoard(ULL *board){
    ULL tempBoard = *board ^(*board - 1);
    unsigned int halfFold = (unsigned) ((tempBoard & 0xffffffff) ^ (tempBoard >> 32));
    *board &= (*board-1);
    return _magic_bit_table[(halfFold * 0x783a9b23) >> 26];
}

vector<int> BitBoardUtils::getIndicesOfBitsInBoard(ULL board){
    vector<int> indices;
    while (board)
        indices.push_back(popLeastSignificantBitFromBoard(&board));
    return indices;
}

vector<int> BitBoardUtils::bitVectorFromULL(ULL board){
    vector<int> bitVector;
    for (int i = 0; i < 8; i++){
        int byte = (board >> 8 * i) & 0xff;
        for (int j = 0; j < 8; j++){
            int bit = (byte >> 1 * j) & 1;
            bitVector.push_back(bit);
        }
    }
    return bitVector;
}

void BitBoardUtils::printBoardOnOneRow(ULL board){
    vector<int> bitVector = bitVectorFromULL(board);
    string boardString;
    for (int i = 63; i >= 0; i--){
        if (i % 8 == 7 && i != 63)
            boardString += "  ";
        if (bitVector[i] == 0)
            boardString += "0";
        else
            boardString += "1";
    }
    cout << endl << boardString << endl;
}

void BitBoardUtils::printBoard(ULL board){
    vector<int> bitVector = bitVectorFromULL(board);
    cout << endl;
    for (int i = 7; i >= 0; i--){
        string rowString;
        for (int j = 0; j < 8; j++){
            if (bitVector[i*8 + j] == 0)
                rowString += " -";
            else
                rowString += " 1";
        }
        cout << rowString << endl;
    }
    cout << endl;
}

ULL BitBoardUtils::getMagicRookMovesForIndex(int index, ULL allPieces){

}




int BitBoardUtils::countBits(ULL bitboard) {
    int count = 0;
    while (bitboard){
        count++;
        bitboard &= bitboard - 1;
    }
    return count;
}

ULL BitBoardUtils::setOccupancy(int count, int bitCount, ULL mask){
    ULL occupancy = 0ULL;
    for (int i = 0; i < bitCount; i++){
        int square = popLeastSignificantBitFromBoard(&mask);
        if (count & (1 << i))
            occupancy |= (1ULL << square);
    }
    return occupancy;
}

ULL** BitBoardUtils::initiateMagicBishopAttacks(){
    ULL** attacks = new ULL*[64];
    for (int square = 0; square < 64; square++){
        attacks[square] = new ULL[512];
        ULL mask = _bishop_move_set[square];
        int bitCount = countBits(mask);
        int occupancyAmount = 1 << bitCount;
        //cout << endl << occupancyAmount << endl;

        //cout << bitCount << endl;

        for (int j = 0; j < occupancyAmount; j++){
            ULL occupancy = setOccupancy(j, bitCount, mask);
            ULL magicIndex = occupancy * _bishop_magics[square] >> 64 - _bishop_relevant_bits[square];
            attacks[square][magicIndex] = bishopAttacksOnTheFly(square, occupancy);
            //cout << magicIndex << " ";
        }
    }
    return attacks;
}

ULL BitBoardUtils::getMagicBishopMovesForSquare(int square, ULL allPieces){

    printBoard(allPieces);

    ULL magicIndex = allPieces & _bishop_move_set[square];

    magicIndex *= _bishop_magics[square];
    magicIndex >>= 64 - _bishop_relevant_bits[square];


    cout << magicIndex;

    ULL moves =  _magic_bishop_attacks[square][magicIndex];

    printBoard(moves);

    return moves;
}

ULL BitBoardUtils::bishopAttacksOnTheFly(int square, ULL block){
    // attack bitboard
    ULL attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }

    // return attack map for bishop on a given square
    return attacks;
}

ULL* BitBoardUtils::generateBishopMoveSet(){
    ULL* attackMap = (ULL*)malloc(64 * sizeof (ULL));

    for (int square = 0; square < 64; square++){
        ULL oneULL = 1;
        ULL squareBit = oneULL << square;
        vector<ULL> moveList;

        for (int i = 7; i % 8  > square % 8; i += 7)
            moveList.push_back(squareBit >> i);
        for (int i = 7; (square + i) % 8  < 7; i += 7)
            moveList.push_back(squareBit << i);
        for (int i = 9+square; i % 8 > square % 8 && i % 8 >= 0; i += 9)
            moveList.push_back(squareBit << i-square);
        for (int i = 9; (i-1) % 8 < square % 8; i += 9)
            moveList.push_back(squareBit >> i);

        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        attackMap[square] = moveBoard;
    }
    return attackMap;
}


