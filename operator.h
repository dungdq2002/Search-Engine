#ifndef OPERATOR_H
#define OPERATOR_H

#include "trie.h"
#include "normalize.h"
#include "library.h"

unordered_map<int, unordered_map<string, int>> handleInput(const string &inputStr, TRIE &trie)
{
    unordered_map<int, unordered_map<string, int>> resultMap;
    vector<string> words = splitInput(inputStr);

    string currentOperator = "";

    for (string word : words)
    {
        if (isOperator(word))
        {
            currentOperator = word;
            continue;
        }

        if (word[0] == '+')
        {
            word.erase(word.begin());
            unordered_map<int, int> wordMap = trie.search(word);
            intersectMap(word, wordMap, resultMap);
            continue;
        }

        if (word[0] == '-')
        {
            word.erase(word.begin());
            unordered_map<int, int> wordMap = trie.search(word);
            eliminateMap(word, wordMap, resultMap);
            continue;
        }

        if (word[0] == '~')
        {
            word.erase(word.begin());
            unordered_map<string, unordered_map<int, int>> synonym = handleSynonym(word);
            if (currentOperator == "" || currentOperator == "OR")
                for (pair<string, unordered_map<int, int>> wordInfo : synonym)
                    mergeMap(wordInfo.first, wordInfo.second, resultMap);
            else
                for (pair<string, unordered_map<int, int>> wordInfo : synonym)
                    intersectMap(wordInfo.first, wordInfo.second, resultMap);
            currentOperator = "";
            continue;
        }

        if(word.find_first_of('*')<=string::npos){
            
        }

        if (word[0] == '\"')
        {
            word.erase(word.begin());
            word.pop_back();
        }
        else if (isStartWiths(word, "intitle:"))
        {
            word.erase(word.begin() + 8);
        }
        else if (isStartWiths(word, "filetype:"))
        {
            word.erase(word.begin() + 9);
        }
        else
        {
        }
    }

    return resultMap;
}

/*unordered_map<int, int> merge(unordered_map<int, int> &A, unordered_map<int, int> &B)
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

unordered_map<int, int> synonym(const string &word, vector<string> &resultWords, TRIE &trie) {}*/

unordered_map<string, unordered_map<int, int>> handleSynonym(const string &word)
{
}

void mergeMap(const string &word, unordered_map<int, int> &wordMap, unordered_map<int, unordered_map<string, int>> &resultMap)
{
    for (pair<int, int> wordInfo : wordMap)
        resultMap[wordInfo.first][word] = wordInfo.second;
}

void intersectMap(const string &word, unordered_map<int, int> &wordMap, unordered_map<int, unordered_map<string, int>> &resultMap)
{
    for (pair<int, int> wordInfo : wordMap)
    {
        unordered_map<int, unordered_map<string, int>>::iterator find = resultMap.find(wordInfo.first);
        if (find != resultMap.end())
            find->second[word] = wordInfo.second;
    }
}

void eliminateMap(const string &word, unordered_map<int, int> &wordMap, unordered_map<int, unordered_map<string, int>> &resultMap)
{
    for (pair<int, int> wordInfo : wordMap)
    {
        unordered_map<int, unordered_map<string, int>>::iterator find = resultMap.find(wordInfo.first);
        if (find != resultMap.end())
            resultMap.erase(find);
    }
}

#endif