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

uint32_t bitAbs(uint32_t absVal){
    uint32_t temp = absVal >> 31;
    absVal ^= temp;
    absVal += temp & 1;
    return absVal;
}

vector<string> splitString(string str, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = str.find (delimiter, pos_start)) != string::npos) {
        token = str.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (str.substr (pos_start));
    return res;
}
