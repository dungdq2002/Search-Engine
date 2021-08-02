#ifndef OPERATOR_H
#define OPERATOR_H

#include "trie.h"
#include "read.h"
#include "normalize.h"

const int titleSize = 10;

unordered_map<int, unordered_map<string, int>> handleInput(const string &inputStr, TRIE &trie, SYNONYM_DATA &synonymData, vector<vector<string>> &fileData)
{
    unordered_map<int, unordered_map<string, int>> resultMap;
    vector<string> keyWords = splitInput(inputStr);
    pair<int, int> oprt = getOperator(keyWords);
    normalizeKeyWords(keyWords);
    switch (oprt.first)
    {
    case 0: //AND

        break;
    case 1: //OR
        break;
    case 2: //*
        break;
    case 3: //intitle:
        break;
    case 4: //filetype:
        break;
    case 5: //#
        break;
    case 6: //~
        break;
    case 7: //+
        break;
    case 8: //-
        break;
    case 9: //$
        break;
    case 10: //" "
        break;
    default:
        break;
    }
}
#endif