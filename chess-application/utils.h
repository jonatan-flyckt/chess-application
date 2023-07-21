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

static const vector<MoveGenerationTestPos> _move_generation_test_positions = {
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
};

#endif // UTILS_H
