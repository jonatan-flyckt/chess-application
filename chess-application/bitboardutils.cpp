#include "bitboardutils.h"

map<int, ULL>  generateKnightAttackSet(){
    map<int, ULL> attackMap;
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
        //printBoard(moveBoard);
        attackMap.insert_or_assign(square, moveBoard);

    }

    return attackMap;
}

map<int, ULL> generateKingAttackSet(){
    map<int, ULL> attackMap;
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
        //printBoard(moveBoard);
        attackMap.insert_or_assign(square, moveBoard);
    }
    return attackMap;
}

map<int, ULL> generateRookAttackSet(){
    map<int, ULL> attackMap;

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
        //printBoard(moveBoard);
        attackMap.insert_or_assign(square, moveBoard);
    }
    return attackMap;

}


map<int, ULL> generateBishopAttackSet(){
    map<int, ULL> attackMap;

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

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap.insert_or_assign(square, moveBoard);
    }
    return attackMap;
}

map<int, ULL> generateQueenAttackSet(){
    map<int, ULL> attackMap;

    for (int square = 0; square < 64; square++){
        vector<ULL> moveList;

        moveList.push_back(_bishop_attack_set[square]);
        moveList.push_back(_rook_attack_set[square]);

        ULL moveBoard = 0;
        for (auto move: moveList) //Store the moves in a bitboard
            moveBoard |= move;

        moveBoard &= 0xffffffffffffffff; //Remove moves outside board

        //cout << _square_from_index[square] << ":" << endl;
        //printBoardOnOneRow(moveBoard);
        //printBoard(moveBoard);
        attackMap.insert_or_assign(square, moveBoard);
    }
    return attackMap;
}

map<int, ULL> generatePawnAttackSet(Colour colour){
    map<int, ULL> attackMap;
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
        attackMap.insert_or_assign(square, moveBoard);
    }
    return attackMap;
}

map<Piece, ULL> generateStartingPosition(){
    map<Piece, ULL> startingBoard;
    ULL oneULL = 1;

    ULL board = 0;
    for (int i = 8; i < 16; i++){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(White, Pawn), board);

    board = 0;
    for (auto i: vector{0, 7}){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(White, Rook), board);

    board = 0;
    for (auto i: vector{1, 6}){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(White, Knight), board);

    board = 0;
    for (auto i: vector{2, 5}){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(White, Bishop), board);

    board = 0;
    board |= oneULL << 3;
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(White, Queen), board);

    board = 0;
    board |= oneULL << 4;
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(White, King), board);


    board = 0;
    for (int i = 48; i < 56; i++){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(Black, Pawn), board);

    board = 0;
    for (auto i: vector{56, 63}){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(Black, Rook), board);

    board = 0;
    for (auto i: vector{57, 62}){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(Black, Knight), board);

    board = 0;
    for (auto i: vector{58, 61}){
        ULL squareBit = oneULL << i;
        board |= squareBit;
    }
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(Black, Bishop), board);

    board = 0;
    board |= oneULL << 59;
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(Black, Queen), board);

    board = 0;
    board |= oneULL << 60;
    printBoardOnOneRow(board);
    printBoard(board);
    startingBoard.insert_or_assign(Piece(Black, King), board);

    return startingBoard;
}

vector<int> bitVectorFromULL(ULL board){
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

void printBoardOnOneRow(ULL board){
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

void printBoard(ULL board){
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



