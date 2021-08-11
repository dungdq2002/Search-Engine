/*#include "screen.h"
#include "read.h"

int main() {
    scanStopword();
    TRIE Trie;
    vector <string> fileName;
    vector <vector<string>> fileData;
    importFile(Trie, "sample-data", fileName, fileData);
    SYNONYM_DATA synonymData;
    synonymData.read_file();
    string word = "get";
    cout << "size " << synonymData.dict[synonymData.idDict[word]].size() << '\n';
    for (auto it: synonymData.dict[synonymData.idDict[word]]) {
        cout << it << '\n';
    }
    exit(0);
    searchBox(Trie, synonymData, fileData);
    // searchBox();
    return 0;
}*/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

typedef pair<int, unordered_map<string, int>> RESULT_PAIR;
typedef unordered_map<int, unordered_map<string, int>> RESULT_MAP;

int getTimeOfAllWords(unordered_map<string, int> &fileInfo)
{
    int result = 0;
    for (pair<string, int> wordInfo : fileInfo)
        result += wordInfo.second;
    return result;
}

bool isBetterResult(RESULT_PAIR &p1, RESULT_PAIR &p2)
{
    int timeOfAllWords1 = getTimeOfAllWords(p1.second);
    int timeOfAllWords2 = getTimeOfAllWords(p2.second);

    if (timeOfAllWords1 > timeOfAllWords2)
        return true;
    if (timeOfAllWords1 < timeOfAllWords2)
        return false;
    return p1.second.size() > p2.second.size();
}

void print5BestResult(RESULT_MAP &resultMap, vector<string> &fileName)
{
    vector<RESULT_PAIR> resultVector;
    for (RESULT_PAIR resultPair : resultMap)
        resultVector.push_back(resultPair);
    sort(resultVector.begin(), resultVector.end(), isBetterResult);
    for (int i = 0; i < 5; ++i)
    {
        cout << "File " << resultVector[i].first << ":\n| ";
        for (pair<string, int> wordInfo : resultVector[i].second)
            cout << wordInfo.first << ": " << wordInfo.second << " | ";
        cout << endl;

        ifstream fin;
        fin.open("sample_data/" + fileName[resultVector[i].first]);
        if (fin.is_open())
        {
        }
    }
}

int main()
{
    RESULT_MAP m;
    srand(time(NULL));
    for (int i = 0; i < 10; ++i)
    {
        int n = rand() % 10;
        for (int j = 0; j < n; ++j)
        {
            string s = to_string(rand() % 1000);
            m[i][s] = rand() % 100;
        }
    }
    for (auto x : m)
    {
        cout << x.first;
        for (auto z : x.second)
            cout << z.first << " " << z.second << endl;
        cout << endl;
    }
    cout << "result:\n";
    //print5BestResult(m);
    return 0;
}