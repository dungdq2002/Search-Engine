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

//Commit cua Duy (Exact, Wildcard)
void exact (unordered_map <int, int> &A, string &key){
    vector<string> v = splitInput(key);
    normalizeKeyWords(v);
    int n = v.size();
    int cnt = 0;
    vector<int> d(n, 0);
    unordered_map<int, int> hashMapKey;
    for (int i=0; i<n; ++i)
        hashMapKey[v[i]] = i;

    for(auto x: A){
        vector<string> *u = &fileData[x.first];
        for (int i=0; i<n; ++i)
            d[i] = 0;
        int m = u.size();

        for (int i=0; i<m; ++i){
            if (hashMapKey.count(u[i])){
                int tmp = hashMapKey[u[i]];
                if (d[tmp] == 0)
                    ++cnt;
                ++d[tmp];
            }

            if (i>=n){
                if (hashMapKey.count(u[i])){
                    int tmp = hashMapKey[u[i]];
                    if (d[tmp] == 1)
                        --cnt;
                    --d[tmp];
                }
            }

            if (cnt == n){
                /// from i-n+1 to i is the exact tring

            }
        }
    }
}

void wildcard (unordered_map <int, int> &A, vector <string> &orderedKey){
    vector<vector<string> > v;
    for (auto x: orderedKey){
        v.push_back(splitInput(x));
        normalizeKeyWords(v.back());
    }

    int cur = 0;
    int numKey = v.size();
    unordered_map<int,int> hashMapKey[numKey];
    vector<int> d[numKey];
    for (int i=0; i<numKey; ++i){
        int n = v[i].size();

        d[i].resize(n);
        for (int j=0; j<n; ++j)
            d[i][j]=0;

        for (int j=0; j<n;++j)
            hashMapKey[i][v[i][j]] = j;
    }

    int cnt = 0;
    for(auto x: A){
        vector<pair<int,int> > res;
        vector<string> *u = &fileData[x.first];
        int m = u.size();

        int cur = 0;
        int n;

        for (int i=0; i<numKey; ++i){
            n = v[i].size();
            for (int j=0; j<n; ++j){
                d[i][j] = 0;
            }
        }

        n = v[cur].size();
        cnt = 0;
        for (int i=0; i<m; ++i){
            if (hashMapKey[cur].count(u[i])){
                int tmp = hashMapKey[cur][u[i]];
                if (d[tmp] == 0)
                    ++cnt;
                ++d[tmp];
            }

            if (i>=n){
                if (hashMapKey[cur].count(u[i])){
                    int tmp = hashMapKey[cur][u[i]];
                    if (d[tmp] == 1)
                        --cnt;
                    --d[tmp];
                }
            }

            if (cnt == n){
                /// from i-n+1 to i is the exact tring
                res.push_back(make_pair(i-n+1, i));
                ++cur;
                if (cur == numKey)
                    break;

                cnt = 0;
                n = v[cur].size();
            }
        }
    }

    /// in Res store to point of the exact string in ordered
    res;

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
