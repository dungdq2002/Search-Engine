#include "screen.h"
#include "read.h"

int main() {
    scanStopword();
    TRIE Trie;
    vector <string> fileName;
    vector <vector<string>> fileData;
    importFile(Trie, "sample-data", fileName, fileData);
    SYNONYM_DATA synonymData;
    synonymData.importData();
    searchBox(Trie, synonymData, fileData, fileName);
    // searchBox();
    return 0;
}

// #include <iostream>
// #include <fstream>
// #include <unordered_map>
// #include <vector>
// #include <algorithm>
// #include <ctime>
// using namespace std;

// typedef pair<int, unordered_map<string, int>> RESULT_PAIR;
// typedef unordered_map<int, unordered_map<string, int>> RESULT_MAP;

// vector<string> splitPharse(const string &inputStr)
// {
//     vector<string> result;
//     size_t startPos = 0, endPos = inputStr.find_first_of(' ');

//     while (endPos <= string::npos)
//     {
//         string tempStr = inputStr.substr(startPos, endPos - startPos);
//         transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
//         result.emplace_back(tempStr);

//         if (endPos == string::npos)
//             return result;

//         startPos = endPos + 1;
//         endPos = inputStr.find_first_of(' ', startPos);
//     }

//     return result;
// }

// int getTimeOfAllWords(unordered_map<string, int> &fileInfo)
// {
//     int result = 0;
//     for (pair<string, int> wordInfo : fileInfo)
//         result += wordInfo.second;
//     return result;
// }

// bool isBetterResult(RESULT_PAIR &p1, RESULT_PAIR &p2)
// {
//     int timeOfAllWords1 = getTimeOfAllWords(p1.second);
//     int timeOfAllWords2 = getTimeOfAllWords(p2.second);

//     if (timeOfAllWords1 > timeOfAllWords2)
//         return true;
//     if (timeOfAllWords1 < timeOfAllWords2)
//         return false;
//     return p1.second.size() > p2.second.size();
// }

// void print5BestResult(RESULT_MAP &resultMap, vector<vector<string>> &fileData)
// {
//     vector<RESULT_PAIR> resultVector;
//     for (RESULT_PAIR resultPair : resultMap)
//         resultVector.push_back(resultPair);

//     sort(resultVector.begin(), resultVector.end(), isBetterResult);

//     for (int i = 0; i < 5; ++i)
//     {
//         int fileID = resultVector[i].first;

//         cout << "File " << fileID << ":\n| ";
//         for (pair<string, int> wordInfo : resultVector[i].second)
//             cout << wordInfo.first << ": " << wordInfo.second << " | ";
//         cout << endl;

//         vector<string> yeu_and_Dung_rat_nhieu = fileData[fileID];

//         for (pair<string, int> Dung_gay : resultVector[i].second)
//             if (Dung_gay.first.find_first_of(" ") == string::npos)
//             {
//                 vector<string>::iterator tim_thay_roi = find(yeu_and_Dung_rat_nhieu.begin(), yeu_and_Dung_rat_nhieu.end(), Dung_gay.first);
//             }
//             else
//             {
//                 vector<string> toi_da_tram_cam = splitPharse(Dung_gay.first);
//                 vector<string>::iterator tim_thay_roi = find(yeu_and_Dung_rat_nhieu.begin(), yeu_and_Dung_rat_nhieu.end(), toi_da_tram_cam[0]);
//             }
//     }
// }

// string completeWildcard(vector<string> &splitedWord, string &wildcard)
// {
//     vector<string>::iterator found = find(splitedWord.begin(), splitedWord.end(), "*");
//     *found = wildcard;
//     string result = "";
//     for (string word : splitedWord)
//         result += word + " ";
//     *found = "*";
//     result.pop_back();
//     return result;
// }

// int main()
// {
//     string s = "dbhvd.vhd";
//     size_t dotPos = s.find_last_of(".");
//     cout << s.substr(dotPos + 1, s.length() - 2 - dotPos) << endl;
//     return 0;
// }