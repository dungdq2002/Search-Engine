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
            unordered_map<int, int> pharseMap = handleExact(wordSplit, trie, fileData);

            if (currentOperator == "" || currentOperator == "OR")
                mergeMap(word, pharseMap, resultMap);
            else
                intersectMap(word, pharseMap, resultMap);

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
        {
            find->second[word] = min(wordInfo.second, find->second[word]);
            if (find->second[word] == 0)
                find->second.erase(word);
        }
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

unordered_map<int, int> handleExact(vector<string> &words, TRIE &trie, vector<vector<string>> &fileData)
{
    unordered_map<int, int> result;
    unordered_map<int, unordered_map<string, int>> allFiles;

    for (string word : words)
    {
        unordered_map<int, int> wordInfo = trie.search(word);
        mergeMap(word, wordInfo, allFiles);
    }

    for (pair<int, unordered_map<string, int>> fileInfo : allFiles)
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

            found = find(found, fileData[fileID].end(), words[0]);
        }
    }

    return result;
}

unordered_map<int, unordered_map<string, int>> handleWildcard(vector<string> words, TRIE &trie, vector<vector<string>> &fileData)
{
    unordered_map<int, unordered_map<string, int>> result;
    unordered_map<int, unordered_map<string, int>> allFiles;

    for (string word : words)
    {
        if (word == "*")
            continue;

        unordered_map<int, int> wordInfo = trie.search(word);
        mergeMap(word, wordInfo, allFiles);
    }

    int wildcardPos = int(find(words.begin(), words.end(), "*") - words.begin());
    int startPos = wildcardPos == 0 ? 1 : 0;

    for (pair<int, unordered_map<string, int>> fileInfo : allFiles)
    {
        if (fileInfo.second.size() < words.size() - 1)
            continue;

        int fileID = fileInfo.first;
        vector<string>::iterator found = find(fileData[fileID].begin(), fileData[fileID].end(), words[startPos]);

        while (found != fileData[fileID].end())
        {
            bool isExact = true;

            for (int i = 2; i < words.size(); ++i)
            {
                if (i == wildcardPos)
                    continue;

                if (*(found + i) != words[i])
                {
                    isExact = false;
                    break;
                }
            }

            if (isExact)
            {
                string wildcard = *(fileData[fileID].begin() + wildcardPos - startPos);
                ++result[fileID][wildcard];
            }

            found = find(found, fileData[fileID].end(), words[startPos]);
        }
    }

    return result;
}

/*Range function
void findRange1(int l, int r){
    vector <int> a;
    findRange2(root, 0, l, r, a);
    cout <<"Number in range: " << l << " to " << r << ": ";
    for(auto x: a) cout << x << " ";
}

void findRange2(TrieNode *root, int cur, int l, int r, vector <int> &d){
    if(root -> isEndOfWord){
        if(l <= cur && cur <= r)
            d.push_back(cur);
    }
    if(cur > r) return;
    if(!root) return;
    for(int i = 0; i <= 9; i++){
        int v = i + '0';
        if(root -> child[v] == nullptr) continue;
        findRange2(root -> child[v], cur * 10 + i, l, r, d);
    }
}
*/
#endif
