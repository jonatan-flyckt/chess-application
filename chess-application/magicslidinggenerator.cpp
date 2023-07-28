#include "magicslidinggenerator.h"

MagicSlidingGenerator::MagicSlidingGenerator(){
    cout << "test" << endl;

    initaliseSliderMasksAndAttacks(Bishop);
    initaliseSliderMasksAndAttacks(Rook);

    //Some tests below
    // create bishop occupancy bitboard
    ULL bishopOccupancy = 0ULL;
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["g7"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["f6"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["c5"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["b2"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["g1"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["a1"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["a8"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["h7"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["g4"]);
    bishopOccupancy = setBit(bishopOccupancy, _index_from_square["d4"]);

    cout << "\n     Bishop occupancy\n";

    // print bishop occupancy
    printBoard(bishopOccupancy);

    cout << "\n     Bishop attacks\n";

    // get bishop attacks
    printBoard(getBishopAttackRays(_index_from_square["d4"], bishopOccupancy));

    // create rook occupancy
    ULL rookOccupancy = 0ULL;
    rookOccupancy = setBit(rookOccupancy, _index_from_square["d7"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["d6"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["d3"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["a4"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["f4"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["a1"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["a8"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["h7"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["g4"]);
    rookOccupancy = setBit(rookOccupancy, _index_from_square["d4"]);

    cout << "\n     Rook occupancy\n";

    // print rook occupancy
    printBoard(rookOccupancy);

    // get rook attacks
    printBoard(getRookAttackRays(_index_from_square["d4"], rookOccupancy));
}

ULL MagicSlidingGenerator::getRookAttackRays(int index, ULL occupancy){
    occupancy &= _rook_masks[index];
    occupancy *=  _rook_magics[index];
    occupancy >>= 64 - _rook_relevant_bits[index];
    return _rook_attacks[index][occupancy];
}

ULL MagicSlidingGenerator::getBishopAttackRays(int index, ULL occupancy){
    occupancy &= _bishop_masks[index];
    occupancy *=  _bishop_magics[index];
    occupancy >>= 64 - _bishop_relevant_bits[index];
    return _bishop_attacks[index][occupancy];
}

void MagicSlidingGenerator::initaliseSliderMasksAndAttacks(PieceType pieceType){
    //loop over 64 board square indices
    for (int index = 0; index < 64; index++){
        //init bishop & rook masks
        _bishop_masks[index] = maskBishopAttacks(index);
        _rook_masks[index] = maskRookAttacks(index);

        // init current mask
        ULL mask = pieceType == Bishop ? maskBishopAttacks(index) : maskRookAttacks(index);

        // count attack mask bits
        int bitCount = countBitsInBoard(mask);

        // occupancy variations count
        int occupancy_variations = 1 << bitCount;

        // loop over occupancy variations, init occupancies, magic index & attacks
        for (int count = 0; count < occupancy_variations; count++){
            if (pieceType == Bishop){
                ULL occupancy = setOccupancy(count, bitCount, mask);
                ULL magic_index = occupancy * _bishop_magics[index] >> (64 - _bishop_relevant_bits[index]);
                _bishop_attacks[index][magic_index] = bishopAttacksInitialisationStage(index, occupancy);
            }
            if (pieceType == Rook){
                ULL occupancy = setOccupancy(count, bitCount, mask);
                ULL magic_index = occupancy * _rook_magics[index] >> (64 - _rook_relevant_bits[index]);
                _rook_attacks[index][magic_index] = rookAttacksInitialisationStage(index, occupancy);
            }
        }
    }
}

ULL MagicSlidingGenerator::setOccupancy(int index, int bitsInMask, ULL attackMask)
{
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
    //init files & ranks
    int f, r;
    //init target files & ranks
    int tr = index / 8;
    int tf = index % 8;

    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attacks |= (1ULL << (r * 8 + f));
    return attacks;
}

ULL MagicSlidingGenerator::maskRookAttacks(int index){
    //return attack map for rook on a given square
    ULL attacks = 0ULL;
    //init files & ranks
    int f, r;
    //init target files & ranks
    int tr = index / 8;
    int tf = index % 8;

    // generate attacks
    for (r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));
    return attacks;
}

ULL MagicSlidingGenerator::bishopAttacksInitialisationStage(int index, ULL blockers){
    //return attack map for bishop on a given square with blockers on the board
    ULL attacks = 0ULL;
    //init files & ranks
    int f, r;
    //init target files & ranks
    int tr = index / 8;
    int tf = index % 8;

    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++){
        attacks |= (1ULL << (r * 8 + f));
        if (blockers & (1ULL << (r * 8 + f))) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--){
        attacks |= (1ULL << (r * 8 + f));
        if (blockers & (1ULL << (r * 8 + f))) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++){
        attacks |= (1ULL << (r * 8 + f));
        if (blockers & (1ULL << (r * 8 + f))) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--){
        attacks |= (1ULL << (r * 8 + f));
        if (blockers & (1ULL << (r * 8 + f))) break;
    }
    return attacks;
}

ULL MagicSlidingGenerator::rookAttacksInitialisationStage(int index, ULL blockers){
    //return attack map for rook on a given square with blockers on the board
    ULL attacks = 0ULL;
    // init files & ranks
    int f, r;
    // init target files & ranks
    int tr = index / 8;
    int tf = index % 8;

    // generate attacks
    for (r = tr + 1; r <= 7; r++){
        attacks |= (1ULL << (r * 8 + tf));
        if (blockers & (1ULL << (r * 8 + tf))) break;
    }
    for (r = tr - 1; r >= 0; r--){
        attacks |= (1ULL << (r * 8 + tf));
        if (blockers & (1ULL << (r * 8 + tf))) break;
    }
    for (f = tf + 1; f <= 7; f++){
        attacks |= (1ULL << (tr * 8 + f));
        if (blockers & (1ULL << (tr * 8 + f))) break;
    }
    for (f = tf - 1; f >= 0; f--){
        attacks |= (1ULL << (tr * 8 + f));
        if (blockers & (1ULL << (tr * 8 + f))) break;
    }
    return attacks;
}

unsigned int MagicSlidingGenerator::generateRandomNumber(){
    // XOR shift algorithm
    unsigned int x = _pseudo_random_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    _pseudo_random_state = x;
    return x;
}

ULL MagicSlidingGenerator::randomULL(){
    ULL u1, u2, u3, u4;
    // randomise numbers
    u1 = (ULL)(generateRandomNumber()) & 0xFFFF;
    u2 = (ULL)(generateRandomNumber()) & 0xFFFF;
    u3 = (ULL)(generateRandomNumber()) & 0xFFFF;
    u4 = (ULL)(generateRandomNumber()) & 0xFFFF;
    // shuffle bits and return
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}
















