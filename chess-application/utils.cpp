#include "utils.h"

string stringFromDifficulty(Difficulty dif){
    if (dif == Easy)
        return "Easy";
    else if (dif == Normal)
        return "Normal";
    else if (dif == Hard)
        return "Hard";
    else
        return "Normal";
}

Difficulty difficultyFromString(string str){
    if (str == "Easy")
        return Easy;
    else if (str == "Normal")
        return Normal;
    else if (str == "Hard")
        return Hard;
    else
        return Normal;
}

string squareIDFromIndices(int row, int col){
    string squareID = "";
    squareID.append(_cols_from_index.at(col));
    squareID.append(to_string(row+1));
    return squareID;
}

pair<int, int> IndicesFromSquareID(string square){
    int row;
    int col;
    row = stoi(square.substr(1, 1))-1;
    for (int i = 0; i < _cols_from_index.size(); i++){
        if (_cols_from_index.at(i) == square.substr(0, 1)){
            col = i;
            break;
        }
    }
    return make_pair(row, col);
}
