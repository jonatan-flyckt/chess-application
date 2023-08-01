#include "magicslidinggenerator.h"

MagicSlidingGenerator::MagicSlidingGenerator(){
    for (int index = 0; index < 64; index++){
        _bishop_masks[index] = maskBishopAttacks(index);
        _rook_masks[index] = maskRookAttacks(index);
    }
    initaliseSliderAttacks(Bishop);
    initaliseSliderAttacks(Rook);

}

ULL MagicSlidingGenerator::getRookAttackRays(const int &index, ULL occupancy){
    occupancy &= _rook_masks[index];
    occupancy *=  _rook_magics[index];
    occupancy >>= _rook_relevant_bits[index];
    return _rook_attacks[index][occupancy];
}

ULL MagicSlidingGenerator::getBishopAttackRays(const int &index, ULL occupancy){
    occupancy &= _bishop_masks[index];
    occupancy *=  _bishop_magics[index];
    occupancy >>= _bishop_relevant_bits[index];
    return _bishop_attacks[index][occupancy];
}

void MagicSlidingGenerator::initaliseSliderAttacks(PieceType pieceType){
    //loop over 64 board square indices
    for (int index = 0; index < 64; index++){
        ULL mask = pieceType == Bishop ? _bishop_masks[index] : _rook_masks[index];
        int bitCount = countBitsInBoard(mask);
        int occupancyVariations = 1 << bitCount;

        // loop over occupancy variations, init occupancies, magic index & attacks
        for (int count = 0; count < occupancyVariations; count++){
            if (pieceType == Bishop){
                ULL occupancy = setOccupancy(count, bitCount, mask);
                ULL magicIndex = occupancy * _bishop_magics[index] >> _bishop_relevant_bits[index];
                _bishop_attacks[index][magicIndex] = bishopAttacksInitialisationStage(index, occupancy);
            }
            if (pieceType == Rook){
                ULL occupancy = setOccupancy(count, bitCount, mask);
                ULL magicIndex = occupancy * _rook_magics[index] >> _rook_relevant_bits[index];
                _rook_attacks[index][magicIndex] = rookAttacksInitialisationStage(index, occupancy);
            }
        }
    }
}

ULL MagicSlidingGenerator::setOccupancy(int index, int bitsInMask, ULL attackMask){
    ULL occupancy = 0ULL;
    for (int count = 0; count < bitsInMask; count++){
        int square = getIndexOfLeastSignificantBit(attackMask);
        attackMask = popBit(attackMask, square);
        //make sure occupancy is on board
        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }
    return occupancy;
}

ULL MagicSlidingGenerator::maskBishopAttacks(int index){
    //return attack map for bishop on a given square
    ULL attacks = 0ULL;
    int file, rank;
    int targetRank = index / 8;
    int targetFile= index % 8;

    //generate attacks
    for (rank = targetRank + 1, file = targetFile+ 1; rank <= 6 && file <= 6; rank++, file++)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = targetRank + 1, file = targetFile- 1; rank <= 6 && file >= 1; rank++, file--)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = targetRank - 1, file = targetFile+ 1; rank >= 1 && file <= 6; rank--, file++)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = targetRank - 1, file = targetFile- 1; rank >= 1 && file >= 1; rank--, file--)
        attacks |= (1ULL << (rank * 8 + file));
    return attacks;
}

ULL MagicSlidingGenerator::maskRookAttacks(int index){
    //return attack map for rook on a given square
    ULL attacks = 0ULL;
    int file, rank;
    int targetRank = index / 8;
    int targetFile= index % 8;

    //generate attacks
    for (rank = targetRank + 1; rank <= 6; rank++)
        attacks |= (1ULL << (rank * 8 + targetFile));
    for (rank = targetRank - 1; rank >= 1; rank--)
        attacks |= (1ULL << (rank * 8 + targetFile));
    for (file = targetFile+ 1; file <= 6; file++)
        attacks |= (1ULL << (targetRank * 8 + file));
    for (file = targetFile- 1; file >= 1; file--)
        attacks |= (1ULL << (targetRank * 8 + file));
    return attacks;
}

ULL MagicSlidingGenerator::bishopAttacksInitialisationStage(int index, ULL blockers){
    //return attack map for bishop on a given square with blockers on the board
    ULL attacks = 0ULL;
    int file, rank;
    int targetRank = index / 8;
    int targetFile= index % 8;

    //generate attacks
    for (rank = targetRank + 1, file = targetFile+ 1; rank <= 7 && file <= 7; rank++, file++){
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file))) break;
    }
    for (rank = targetRank + 1, file = targetFile- 1; rank <= 7 && file >= 0; rank++, file--){
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file))) break;
    }
    for (rank = targetRank - 1, file = targetFile+ 1; rank >= 0 && file <= 7; rank--, file++){
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file))) break;
    }
    for (rank = targetRank - 1, file = targetFile- 1; rank >= 0 && file >= 0; rank--, file--){
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file))) break;
    }
    return attacks;
}

ULL MagicSlidingGenerator::rookAttacksInitialisationStage(int index, ULL blockers){
    //return attack map for rook on a given square with blockers on the board
    ULL attacks = 0ULL;
    int file, rank;
    int targetRank = index / 8;
    int targetFile= index % 8;

    //generate attacks
    for (rank = targetRank + 1; rank <= 7; rank++){
        attacks |= (1ULL << (rank * 8 + targetFile));
        if (blockers & (1ULL << (rank * 8 + targetFile))) break;
    }
    for (rank = targetRank - 1; rank >= 0; rank--){
        attacks |= (1ULL << (rank * 8 + targetFile));
        if (blockers & (1ULL << (rank * 8 + targetFile))) break;
    }
    for (file = targetFile+ 1; file <= 7; file++){
        attacks |= (1ULL << (targetRank * 8 + file));
        if (blockers & (1ULL << (targetRank * 8 + file))) break;
    }
    for (file = targetFile- 1; file >= 0; file--){
        attacks |= (1ULL << (targetRank * 8 + file));
        if (blockers & (1ULL << (targetRank * 8 + file))) break;
    }
    return attacks;
}

unsigned int MagicSlidingGenerator::generateRandomNumber(){
    //XOR shift algorithm
    unsigned int x = _pseudo_random_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    _pseudo_random_state = x;
    return x;
}

ULL MagicSlidingGenerator::randomULL(){
    ULL u1, u2, u3, u4;
    //randomise numbers
    u1 = (ULL)(generateRandomNumber()) & 0xFFFF;
    u2 = (ULL)(generateRandomNumber()) & 0xFFFF;
    u3 = (ULL)(generateRandomNumber()) & 0xFFFF;
    u4 = (ULL)(generateRandomNumber()) & 0xFFFF;
    //shuffle bits and return
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}
























