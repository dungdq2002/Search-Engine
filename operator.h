#ifndef OPERATOR_H
#define OPERATOR_H

#include "trie.h"
#include "read.h"
#include "normalize.h"

#include <vector>

const int titleSize = 10;

unordered_map<int, unordered_map<string, int>> handleInput(const string &inputStr, TRIE &trie, SYNONYM_DATA &synonymData, vector<vector<string>> &fileData)
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
            unordered_map<string, unordered_map<int, int>> synonym = handleSynonym(word, trie, synonymData);

            if (currentOperator == "" || currentOperator == "OR")
                for (pair<string, unordered_map<int, int>> wordInfo : synonym)
                    mergeMap(wordInfo.first, wordInfo.second, resultMap);

            else
                for (pair<string, unordered_map<int, int>> wordInfo : synonym)
                    intersectMap(wordInfo.first, wordInfo.second, resultMap);

            currentOperator = "";
            continue;
        }

        if (word.find_first_of('*') <= string::npos)
        {
            vector<string> wordSplit = splitPharse(word);

            continue;
        }

        if (word[0] == '\"')
        {
            word.erase(word.begin());
            word.pop_back();
            vector<string> wordSplit = splitPharse(word);

            continue;
        }

        if (isStartWiths(word, "intitle:"))
        {
            word.erase(word.begin() + 8);
            unordered_map<string, unordered_map<int, int>> wordMap = handleIntitle(word, trie, fileData);

            if (currentOperator == "" || currentOperator == "OR")
                for (pair<string, unordered_map<int, int>> wordInfo : wordMap)
                    mergeMap(wordInfo.first, wordInfo.second, resultMap);

            else
                for (pair<string, unordered_map<int, int>> wordInfo : wordMap)
                    intersectMap(wordInfo.first, wordInfo.second, resultMap);

            currentOperator = "";
            continue;
        }

        if (isStartWiths(word, "filetype:"))
        {
            word.erase(word.begin() + 9);

            continue;
        }

        unordered_map<int, int> wordMap = trie.search(word);

        if (currentOperator == "" || currentOperator == "OR")
            mergeMap(word, wordMap, resultMap);
        else
            intersectMap(word, wordMap, resultMap);

        currentOperator = "";
    }

    return resultMap;
}

unordered_map<string, unordered_map<int, int>> handleSynonym(const string &word, TRIE &trie, SYNONYM_DATA &synonymData)
{
    unordered_map<string, unordered_map<int, int>> result;

    for (string synonymWord : synonymData.dict[synonymData.idDict[word]])
        result[synonymWord] = trie.search(synonymWord);

    return result;
}

unordered_map<string, unordered_map<int, int>> handleIntitle(const string &pharse, TRIE &trie, vector<vector<string>> &fileData)
{
    unordered_map<string, unordered_map<int, int>> result;
    vector<string> pharseSplit = splitPharse(pharse);

    for (string word : pharseSplit)
    {
        unordered_map<int, int> wordMap = trie.search(word);

        for (pair<int, int> wordInfo : wordMap)
        {
            int countWord = count(fileData[wordInfo.first].begin(), fileData[wordInfo.first].begin() + titleSize, word);
            if (countWord > 0)
                result[word][wordInfo.first] = countWord;
        }
    }
}

void mergeMap(const string &word, unordered_map<int, int> &wordMap, unordered_map<int, unordered_map<string, int>> &resultMap)
{
    for (pair<int, int> wordInfo : wordMap)
        resultMap[wordInfo.first][word] = max(wordInfo.second, resultMap[wordInfo.first][word]);
}

void intersectMap(const string &word, unordered_map<int, int> &wordMap, unordered_map<int, unordered_map<string, int>> &resultMap)
{
    for (pair<int, int> wordInfo : wordMap)
    {
        unordered_map<int, unordered_map<string, int>>::iterator find = resultMap.find(wordInfo.first);

        if (find != resultMap.end())
            find->second[word] = min(wordInfo.second, find->second[word]);
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
