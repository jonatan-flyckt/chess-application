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

        cout << _square_from_index[square] << ":" << endl;
        printBoard(moveBoard);
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

        cout << _square_from_index[square] << ":" << endl;
        printBoard(moveBoard);
        attackMap.insert_or_assign(square, moveBoard);
    }
    return attackMap;
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

