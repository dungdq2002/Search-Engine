#ifndef OPERATOR_H
#define OPERATOR_H

#include "trie.h"
#include "read.h"
#include "normalize.h"

#include <vector>

const int titleSize = 10;

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

    return result;
}

void mergeMap(const string &word, unordered_map<int, int> &wordMap, RESULT_MAP &resultMap)
{
    for (pair<int, int> wordInfo : wordMap)
        resultMap[wordInfo.first][word] = max(wordInfo.second, resultMap[wordInfo.first][word]);
}

void intersectMap(const string &word, unordered_map<int, int> &wordMap, RESULT_MAP &resultMap)
{
    for (RESULT_PAIR fileInfo : resultMap)
    {
        int fileID = fileInfo.first;
        unordered_map<int, int>::iterator found = wordMap.find(fileID);
        if (found != wordMap.end())
        {
            if (resultMap[fileID].find(word) == resultMap[fileID].end())
                resultMap[fileID][word] = wordMap[fileID];
        }
        else
            resultMap.erase(fileID);
    }
}

void eliminateMap(const string &word, unordered_map<int, int> &wordMap, RESULT_MAP &resultMap)
{
    for (pair<int, int> wordInfo : wordMap)
    {
        RESULT_MAP::iterator find = resultMap.find(wordInfo.first);

        if (find != resultMap.end())
            resultMap.erase(find);
    }
}

unordered_map<int, int> handleExact(vector<string> &words, TRIE &trie, vector<vector<string>> &fileData)
{
    unordered_map<int, int> result;
    RESULT_MAP allFiles;

    for (string word : words)
    {
        unordered_map<int, int> wordInfo = trie.search(word);
        mergeMap(word, wordInfo, allFiles);
    }

    for (RESULT_PAIR fileInfo : allFiles)
    {
        if (fileInfo.second.size() < words.size())
            continue;

        int fileID = fileInfo.first;
        vector<string>::iterator found = find(fileData[fileID].begin(), fileData[fileID].end(), words[0]);

        while (found != fileData[fileID].end())
        {
            bool isExact = true;

            for (int i = 1; i < words.size(); ++i)
                if (*(found + i) != words[i])
                {
                    isExact = false;
                    break;
                }

            if (isExact)
                ++result[fileID];

            found = find(found + 1, fileData[fileID].end(), words[0]);
        }
    }

    return result;
}

RESULT_MAP handleWildcard(vector<string> words, TRIE &trie, vector<vector<string>> &fileData)
{
    RESULT_MAP result;
    RESULT_MAP allFiles;

    for (string word : words)
    {
        if (word == "*")
            continue;

        unordered_map<int, int> wordInfo = trie.search(word);
        mergeMap(word, wordInfo, allFiles);
    }

    int wildcardPos = int(find(words.begin(), words.end(), "*") - words.begin());
    int startPos = wildcardPos == 0 ? 1 : 0;

    for (RESULT_PAIR fileInfo : allFiles)
    {
        if (fileInfo.second.size() < words.size() - 1)
            continue;

        int fileID = fileInfo.first;
        vector<string>::iterator found = find(fileData[fileID].begin(), fileData[fileID].end(), words[startPos]);

        while (found != fileData[fileID].end())
        {
            bool isExact = true;

            for (int i = startPos + 1; i < words.size(); ++i)
            {
                if (found + i == words.end())
                {
                    isExact = false;
                    break;
                }

                if (i == wildcardPos)
                    continue;

                if (*(found + i) != words[i])
                {
                    // cout << *(found + i) << ' ' << words[i] << '\n';
                    isExact = false;
                    break;
                }
            }

            if (isExact)
            {
                string wildcard = *(found + wildcardPos - startPos);
                ++result[fileID][wildcard];
            }

            // cout << "im here " << isExact << '\n';
            found = find(found + 1, fileData[fileID].end(), words[startPos]);
        }
    }

    return result;
}

string completeWildcard(vector<string> &splitedWord, string &wildcard)
{
    vector<string>::iterator found = find(splitedWord.begin(), splitedWord.end(), "*");
    *found = wildcard;

    string result = "";
    for (string word : splitedWord)
        result += word + " ";

    *found = "*";

    result.pop_back();
    return result;
}

string getFiletype(int &fileID, vector<string> &fileName)
{
    size_t dotPos = fileName[fileID].find_last_of(".");
    return fileName[fileID].substr(dotPos + 1, fileName[fileID].length() - 1 - dotPos);
}

RESULT_MAP handleInput(const string &inputStr, TRIE &trie, SYNONYM_DATA &synonymData, vector<vector<string>> &fileData, vector<string> &fileName)
{
    RESULT_MAP resultMap;
    vector<string> words = splitInput(inputStr);

    string currentOperator = "";
    string filetype = "";

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

        if (word.find_first_of("..") < string::npos) {
            RESULT_MAP pharseMap;
            int pos = word.find_first_of("..");
            string left = word.substr(0, pos);
            string right = word.substr(pos+2, word.size() - (pos + 2));
            // clean(left), clean(right);
            trie.inRange(left, right, pharseMap);
            if (currentOperator == "" || currentOperator == "OR") {
                // merge pha xe` voi res
                for (auto PAIR : pharseMap) {
                    for (auto MAP : PAIR.second)
                        resultMap[PAIR.first][MAP.first] += MAP.second;
                }
            }
            else {
                // intersect phe xe` voi res
                for (auto PAIR : resultMap) {
                    if (pharseMap.find(PAIR.first) == pharseMap.end()) {
                        resultMap.erase(PAIR.first);
                    }
                    else {
                        for (auto MAP : pharseMap[PAIR.first])
                            resultMap[PAIR.first][MAP.first] += MAP.second;
                    }
                }
            }
        }

        if (word.find_first_of('*') < string::npos)
        {
            vector<string> wordSplit = splitPharse(word);

            RESULT_MAP pharseMap = handleWildcard(wordSplit, trie, fileData);

            if (currentOperator == "" || currentOperator == "OR")
                for (RESULT_PAIR fileInfo : pharseMap)
                    for (pair<string, int> wordInfo : fileInfo.second)
                        resultMap[fileInfo.first][completeWildcard(wordSplit, wordInfo.first)] = wordInfo.second;
            else
                for (RESULT_PAIR fileInfo : resultMap)
                {
                    // RESULT_MAP::iterator found = find(pharseMap.begin(), pharseMap.end(), fileInfo.first);
                    int huhu = fileInfo.first;
                    auto found = pharseMap.find(huhu);
                    // RESULT_MAP::iterator found = pharseMap.begin();

                    if (found != pharseMap.end())
                        for (pair<string, int> wordInfo : found->second)
                            resultMap[fileInfo.first][completeWildcard(wordSplit, wordInfo.first)] = wordInfo.second;
                    else
                        resultMap.erase(fileInfo.first);
                }

            continue;
        }

        if (word[0] == '\"')
        {
            word.erase(word.begin());
            word.pop_back();
            vector<string> wordSplit = splitPharse(word);
            unordered_map<int, int> pharseMap = handleExact(wordSplit, trie, fileData);
            if (currentOperator == "" || currentOperator == "OR")
                mergeMap(word, pharseMap, resultMap);
            else
                intersectMap(word, pharseMap, resultMap);

            continue;
        }

        if (isStartWiths(word, "intitle:"))
        {
            word.erase(word.begin(), word.begin() + 8);
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
            word.erase(word.begin(), word.begin() + 9);
            filetype = word;

            continue;
        }

        unordered_map<int, int> wordMap = trie.search(word);

        if (currentOperator == "" || currentOperator == "OR")
            mergeMap(word, wordMap, resultMap);
        else
            intersectMap(word, wordMap, resultMap);

        currentOperator = "";
    }

    if (filetype != "")
    {
        for (RESULT_PAIR fileInfo : resultMap)
            if (getFiletype(fileInfo.first, fileName) != filetype)
                resultMap.erase(fileInfo.first);
    }

    return resultMap;
}

#endif
