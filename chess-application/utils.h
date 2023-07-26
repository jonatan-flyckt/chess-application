#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "chessstructs.h"
#include <tuple>
#include <iostream>


typedef uint64_t ULL;

using namespace std;

static const vector<string> _cols_from_index{"a", "b", "c", "d", "e", "f", "g", "h"};

string stringFromDifficulty(Difficulty dif);

Difficulty difficultyFromString(string str);

string squareIDFromIndices(int row, int col);

pair<int, int> IndicesFromSquareID(string square);

uint32_t bitAbs(uint32_t num);

vector<string> splitString(string str, string delimiter);

bool boardContainsExactlyOnePiece(ULL board);

static const Piece _white_pawn_piece_const = Piece(White, Pawn);
static const Piece _white_rook_piece_const = Piece(White, Rook);
static const Piece _white_bishop_piece_const = Piece(White, Bishop);
static const Piece _white_knight_piece_const = Piece(White, Knight);
static const Piece _white_queen_piece_const = Piece(White, Queen);
static const Piece _white_king_piece_const = Piece(White, King);

static const Piece _black_pawn_piece_const = Piece(Black, Pawn);
static const Piece _black_rook_piece_const = Piece(Black, Rook);
static const Piece _black_bishop_piece_const = Piece(Black, Bishop);
static const Piece _black_knight_piece_const = Piece(Black, Knight);
static const Piece _black_queen_piece_const = Piece(Black, Queen);
static const Piece _black_king_piece_const = Piece(Black, King);

//Move generation test positions and their correct results defined below:
struct MoveGenerationTestPos{
    MoveGenerationTestPos(string fen, string description, int depth, map<string, int> *correctSplitResults){
        _fen = fen;
        _description = description;
        _depth = depth;
        _correct_split_results = correctSplitResults;
    };
    string _fen;
    string _description;
    int _depth;
    map<string, int> *_correct_split_results;
};

static const vector<MoveGenerationTestPos> _move_generation_test_positions_small = {
    {
        MoveGenerationTestPos(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "Starting position",
        3,
        new map<string, int>{{"a2a3", 380}, {"b2b3", 420}, {"c2c3", 420}, {"d2d3", 539}, {"e2e3", 599},
                            {"f2f3", 380}, {"g2g3", 420}, {"h2h3", 380}, {"a2a4", 420}, {"b2b4", 421},
                            {"c2c4", 441}, {"d2d4", 560}, {"e2e4", 600}, {"f2f4", 401}, {"g2g4", 421},
                            {"h2h4", 420}, {"b1a3", 400}, {"b1c3", 440}, {"g1f3", 440}, {"g1h3", 400}}
        )
    },
    {
        MoveGenerationTestPos(
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",
        "Mid-game position with many active pieces, checks, captures, and castling rights",
        2,
        new map<string, int>{{"a2a3", 44}, {"b2b3", 42}, {"g2g3", 42}, {"d5d6", 41}, {"a2a4", 44},
                             {"g2g4", 42}, {"g2h3", 43}, {"d5e6", 46}, {"e5d3", 43}, {"e5c4", 42},
                             {"e5g4", 44}, {"e5c6", 41}, {"e5g6", 42}, {"e5d7", 45}, {"e5f7", 44},
                             {"c3b1", 42}, {"c3d1", 42}, {"c3a4", 42}, {"c3b5", 39}, {"d2c1", 43},
                             {"d2e3", 43}, {"d2f4", 43}, {"d2g5", 42}, {"d2h6", 41}, {"e2d1", 44},
                             {"e2f1", 44}, {"e2d3", 42}, {"e2c4", 41}, {"e2b5", 39}, {"e2a6", 36},
                             {"a1b1", 43}, {"a1c1", 43}, {"a1d1", 43}, {"h1f1", 43}, {"h1g1", 43},
                             {"f3d3", 42}, {"f3e3", 43}, {"f3g3", 43}, {"f3h3", 43}, {"f3f4", 43},
                             {"f3g4", 43}, {"f3f5", 45}, {"f3h5", 43}, {"f3f6", 39}, {"e1d1", 43},
                             {"e1f1", 43}, {"e1g1", 43}, {"e1c1", 43}
        }
        )
    },
    {
        MoveGenerationTestPos(
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ",
        "End-game position with discovered checks and some en passants",
        3,
        new map<string, int>{{"e2e3", 205}, {"g2g3", 54}, {"a5a6", 240}, {"e2e4", 177}, {"g2g4", 226},
                             {"b4b1", 265}, {"b4b2", 205}, {"b4b3", 248}, {"b4a4", 202}, {"b4c4", 254},
                             {"b4d4", 243}, {"b4e4", 228}, {"b4f4", 41}, {"a5a4", 224}
        }
        )
    },
    {
        MoveGenerationTestPos(
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "Mid-game position with active check and many promotions",
        3,
        new map<string, int>{{"g1h1", 1753}, {"c4c5", 1409}, {"d2d4", 1643}, {"f3d4", 1687}, {"b4c5", 1352},
                             {"f1f2", 1623}
        }
        )
    },
    {
        MoveGenerationTestPos(
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ",
        "Tricky mid-game position that many engines fail with",
        2,
        new map<string, int>{{"a2a3", 34}, {"b2b3", 34}, {"c2c3", 34}, {"g2g3", 34}, {"h2h3", 34},
                             {"a2a4", 34}, {"b2b4", 33}, {"g2g4", 34}, {"h2h4", 34}, {"d7c8", 144},
                             {"e2g1", 34}, {"e2c3", 34}, {"e2g3", 34}, {"e2d4", 34}, {"e2f4", 34},
                             {"b1d2", 34}, {"b1a3", 34}, {"b1c3", 34}, {"c4b3", 34}, {"c4d3", 34},
                             {"c4b5", 34}, {"c4d5", 35}, {"c4a6", 33}, {"c4e6", 35}, {"c4f7", 32},
                             {"c1d2", 34}, {"c1e3", 34}, {"c1f4", 34}, {"c1g5", 32}, {"c1h6", 31},
                             {"h1f1", 34}, {"h1g1", 34}, {"d1d2", 34}, {"d1d3", 34}, {"d1d4", 34},
                             {"d1d5", 35}, {"d1d6", 28}, {"e1f1", 34}, {"e1d2", 34}, {"e1f2", 28},
                             {"e1g1", 34}
        }
        )
    }
};


static const vector<MoveGenerationTestPos> _move_generation_test_positions_large = {
    {
        MoveGenerationTestPos(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "Starting position",
        4,
        new map<string, int>{{"a2a3", 8457}, {"b2b3", 9345}, {"c2c3", 9272}, {"d2d3", 11959}, {"e2e3", 13134},
                            {"f2f3", 8457}, {"g2g3", 9345}, {"h2h3", 8457}, {"a2a4", 9329}, {"b2b4", 9332},
                            {"c2c4", 9744}, {"d2d4", 12435}, {"e2e4", 13160}, {"f2f4", 8929}, {"g2g4", 9328},
                            {"h2h4", 9329}, {"b1a3", 8885}, {"b1c3", 9755}, {"g1f3", 9748}, {"g1h3", 8881}}
        )
    },
    {
        MoveGenerationTestPos(
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",
        "Mid-game position with many active pieces, checks, captures, and castling rights",
        3,
        new map<string, int>{{"a2a3", 2186}, {"b2b3", 1964}, {"g2g3", 1882}, {"d5d6", 1991}, {"a2a4", 2149},
                             {"g2g4", 1843}, {"g2h3", 1970}, {"d5e6", 2241}, {"e5d3", 1803}, {"e5c4", 1880},
                             {"e5g4", 1878}, {"e5c6", 2027}, {"e5g6", 1997}, {"e5d7", 2124}, {"e5f7", 2080},
                             {"c3b1", 2038}, {"c3d1", 2040}, {"c3a4", 2203}, {"c3b5", 2138}, {"d2c1", 1963},
                             {"d2e3", 2136}, {"d2f4", 2000}, {"d2g5", 2134}, {"d2h6", 2019}, {"e2d1", 1733},
                             {"e2f1", 2060}, {"e2d3", 2050}, {"e2c4", 2082}, {"e2b5", 2057}, {"e2a6", 1907},
                             {"a1b1", 1969}, {"a1c1", 1968}, {"a1d1", 1885}, {"h1f1", 1929}, {"h1g1", 2013},
                             {"f3d3", 2005}, {"f3e3", 2174}, {"f3g3", 2214}, {"f3h3", 2360}, {"f3f4", 2132},
                             {"f3g4", 2169}, {"f3f5", 2396}, {"f3h5", 2267}, {"f3f6", 2111}, {"e1d1", 1894},
                             {"e1f1", 1855}, {"e1g1", 2059}, {"e1c1", 1887}
        }
        )
    },
    {
        MoveGenerationTestPos(
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ",
        "End-game position with discovered checks and some en passants",
        4,
        new map<string, int>{{"e2e3", 3107}, {"g2g3", 1014}, {"a5a6", 3653}, {"e2e4", 2748}, {"g2g4", 3702},
                             {"b4b1", 4199}, {"b4b2", 3328}, {"b4b3", 3658}, {"b4a4", 3019}, {"b4c4", 3797},
                             {"b4d4", 3622}, {"b4e4", 3391}, {"b4f4", 606}, {"a5a4", 3394}
        }
        )
    },
    {
        MoveGenerationTestPos(
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "Mid-game position with active check and many promotions",
        4,
        new map<string, int>{{"g1h1", 81638}, {"c4c5", 60769}, {"d2d4", 72051}, {"f3d4", 75736}, {"b4c5", 58167},
                             {"f1f2", 73972}
        }
        )
    },
    {
        MoveGenerationTestPos(
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ",
        "Tricky mid-game position that many engines fail with",
        3,
        new map<string, int>{{"a2a3", 1373}, {"b2b3", 1368}, {"c2c3", 1440}, {"g2g3", 1308}, {"h2h3", 1371},
                             {"a2a4", 1433}, {"b2b4", 1398}, {"g2g4", 1337}, {"h2h4", 1402}, {"d7c8", 6030},
                             {"e2g1", 1431}, {"e2c3", 1595}, {"e2g3", 1523}, {"e2d4", 1554}, {"e2f4", 1555},
                             {"b1d2", 1174}, {"b1a3", 1303}, {"b1c3", 1467}, {"c4b3", 1275}, {"c4d3", 1269},
                             {"c4b5", 1332}, {"c4d5", 1375}, {"c4a6", 1256}, {"c4e6", 1438}, {"c4f7", 1328},
                             {"c1d2", 1368}, {"c1e3", 1587}, {"c1f4", 1552}, {"c1g5", 1422}, {"c1h6", 1312},
                             {"h1f1", 1364}, {"h1g1", 1311}, {"d1d2", 1436}, {"d1d3", 1685}, {"d1d4", 1751},
                             {"d1d5", 1688}, {"d1d6", 1500}, {"e1f1", 1445}, {"e1d2", 978}, {"e1f2", 1269},
                             {"e1g1", 1376}
        }
        )
    }
};

#endif // UTILS_H
