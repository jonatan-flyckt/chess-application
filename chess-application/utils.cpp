#include "utils.h"

Utils::Utils(){

}

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
