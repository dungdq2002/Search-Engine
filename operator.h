#ifndef OPERATOR_H
#define OPERATOR_H

#include "trie.h"
#include "normalize.h"
#include "library.h"

void handleInput(const string &inputStr, TRIE &trie)
{
    vector<string> resultWords, eliminatedWords;
    unordered_map<int, int> resultMap;

    vector<string> words = splitString(inputStr);
    string currentOperator = "";

    for (string word : words)
    {
        if (word == "AND" || word == "OR")
        {
            currentOperator = word;
            continue;
        }
        pair<int, unordered_map<int, int>> handle = handleWord(word, resultWords, eliminatedWords, trie);
        if (handle.first == 1)
        {
            resultMap = merge(resultMap, handle.second);
            continue;
        }
        else if (handle.first == -1)
        {
        }
        else
        {
            if (currentOperator == "AND")
                resultMap = merge(resultMap, handle.second);
            else
                resultMap = intersect(resultMap, handle.second);
            currentOperator = "";
        }
    }
    //return resultMap;
}

pair<int, unordered_map<int, int>> handleWord(string &word, vector<string> &resultWords, vector<string> &eliminatedWords, TRIE &trie)
{
    if (isAlphaOrDigit(word[0]) || word[0] == '$' || word[0] == '#')
    {
        resultWords.push_back(word);
        return make_pair(0, trie.search(word));
    }
    if (word[0] == '~')
    {
        word.erase(word.begin());
        return make_pair(0, synonym(word, resultWords, trie));
    }
    int oprt;
    if (word[0] == '+')
        oprt = 1;
    else
        oprt = -1;
    word.erase(word.begin());
    return make_pair(oprt, trie.search(word));
}

unordered_map<int, int> merge(unordered_map<int, int> &A, unordered_map<int, int> &B)
{
    unordered_map<int, int> result = A;
    for (pair<int, int> b : B)
        result[b.first] += b.second;
    return result;
}

unordered_map<int, int> intersect(unordered_map<int, int> &A, unordered_map<int, int> &B)
{
    unordered_map<int, int> result;
    for (pair<int, int> a : A)
    {
        unordered_map<int, int>::iterator find = B.find(a.first);
        if (find != B.end())
            result[a.first] = a.second + (*find).second;
    }
    return result;
}

unordered_map<int, int> fileNameQuery(unordered_map<int, int> &A, string &fileNameRequire)
{
}

unordered_map<int, int> synonym(const string &word, vector<string> &resultWords, TRIE &trie)
{
}

#endif