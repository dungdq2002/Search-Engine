<<<<<<< HEAD
#include "screen.h"
#include "read.h"

int main() {
    scanStopword();
    TRIE Trie;
    vector <string> fileName;
    vector <vector<string>> fileData;
    importFile(Trie, "Search Engine-Data", fileName, fileData);
    SYNONYM_DATA synonymData;
    synonymData.importData();
    searchBox(Trie, synonymData, fileData, fileName);
    // searchBox();
    return 0;
}

/*#include <iostream>
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

vector<string> splitPharse(const string &inputStr)
{
    vector<string> result;
    size_t startPos = 0, endPos = inputStr.find_first_of(' ');

    while (endPos <= string::npos)
    {
        string tempStr = inputStr.substr(startPos, endPos - startPos);
        transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
        result.emplace_back(tempStr);

        if (endPos == string::npos)
            return result;

        startPos = endPos + 1;
        endPos = inputStr.find_first_of(' ', startPos);
    }

    return result;
}

void print5BestResult(RESULT_MAP &resultMap, vector<vector<string>> &fileData)
{
    vector<RESULT_PAIR> resultVector;
    for (RESULT_PAIR resultPair : resultMap)
        resultVector.push_back(resultPair);

    sort(resultVector.begin(), resultVector.end(), isBetterResult);

    for (int i = 0; i < 5; ++i)
    {
        int fileID = resultVector[i].first;

        cout << "File " << fileID << ":\n| ";
        for (pair<string, int> wordInfo : resultVector[i].second)
            cout << wordInfo.first << ": " << wordInfo.second << " | ";
        cout << endl;

        vector<string> yeu_and_Dung_rat_nhieu = fileData[fileID];

        for (pair<string, int> Dung_gay : resultVector[i].second)
            if (Dung_gay.first.find_first_of(" ") == string::npos)
            {
                vector<string>::iterator tim_thay_roi = find(yeu_and_Dung_rat_nhieu.begin(), yeu_and_Dung_rat_nhieu.end(), Dung_gay.first);
            }
            else
            {
                vector<string> toi_da_tram_cam = splitPharse(Dung_gay.first);
                vector<string>::iterator tim_thay_roi = find(yeu_and_Dung_rat_nhieu.begin(), yeu_and_Dung_rat_nhieu.end(), toi_da_tram_cam[0]);
            }
    }
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

bool isAlphaOrDigit(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '0');
}

bool isOperator(const string &word)
{
    return word == "AND" || word == "OR";
}

bool isStartWiths(const string &word, const string &chars)
{
    return word.substr(0, chars.length()) == chars;
}

bool hasOperator(const string &word)
{
    if (isStartWiths(word, "intitle:") || isStartWiths(word, "filetype"))
        return true;

    vector<char> oprts = {'+', '-', '$', '#', '\"'};
    for (char oprt : oprts)
        if (word[0] == oprt)
            return true;

    if (word.find_first_of("..") != string::npos || word.find_first_of("*") != string::npos)
        return true;
    return false;
}

vector<string> splitInput(const string &inputStr)
{
    vector<string> result;
    size_t startPos = 0, endPos = inputStr.find_first_of(' ');

    while (endPos <= string::npos)
    {
        if (inputStr[startPos] == '\"')
        {
            endPos = inputStr.find_first_of('\"', startPos + 1);
            result.emplace_back(inputStr.substr(startPos, endPos - startPos + 1));
            if (endPos == string::npos)
            {
                return result;
            }
            startPos = endPos + 1;
            endPos = inputStr.find_first_of(' ', startPos);
            continue;
        }

        string tempStr = inputStr.substr(startPos, endPos - startPos);

        if (tempStr == "AND" || tempStr == "OR")
        {
            result.emplace_back(tempStr);
        }

        else if (tempStr == "*")
        {
            while (!result.empty())
            {
                string tempWord = result[result.size() - 1];

                if (!isOperator(tempWord) && !hasOperator(tempWord))
                {
                    tempStr = tempWord + " " + tempStr;
                    result.pop_back();
                }
                else
                    break;
            }

            startPos = endPos + 1;
            endPos = inputStr.find_first_of(' ', startPos);

            while (endPos <= string::npos)
            {
                string tempTempStr = inputStr.substr(startPos, endPos - startPos);

                if (!isOperator(tempTempStr) && !hasOperator(tempTempStr))
                    tempStr += " " + tempTempStr;
                else
                {
                    startPos -= tempTempStr.length() + 1;
                    endPos -= tempTempStr.length() + 1;
                    break;
                }

                if (endPos == string::npos)
                {
                    result.emplace_back(tempStr);
                    for (int i = 0; i < result.size(); ++i)
                        for (int j = 0; j < result[i].length(); ++j)
                            if (result[i][j] == ' ')
                                result[i][j] = '1';
                    return result;
                }

                startPos = endPos + 1;
                endPos = inputStr.find_first_of(' ', startPos);
            }

            result.emplace_back(tempStr);
        }

        else if (isStartWiths(tempStr, "intitle:"))
        {
            if (endPos == string::npos)
            {
                result.emplace_back(tempStr);

                return result;
            }
            startPos = endPos + 1;
            endPos = inputStr.find_first_of(' ', startPos);

            while (endPos <= string::npos)
            {
                string tempTempStr = inputStr.substr(startPos, endPos - startPos);

                if (!isOperator(tempTempStr) && !hasOperator(tempTempStr))
                {
                    cout << "yes\n";
                    tempStr += " " + tempTempStr;
                }
                else
                {
                    startPos -= tempTempStr.length() + 1;
                    endPos -= tempTempStr.length() + 1;
                    break;
                }
                //cout<<tempStr<<endl;
                if (endPos == string::npos)
                {
                    result.emplace_back(tempStr);

                    return result;
                }

                startPos = endPos + 1;
                endPos = inputStr.find_first_of(' ', startPos);
            }

            result.emplace_back(tempStr);
        }

        else if (tempStr != " " && tempStr != "")
        {
            transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
            result.emplace_back(tempStr);
        }

        if (endPos == string::npos)
            return result;

        startPos = endPos + 1;
        endPos = inputStr.find_first_of(' ', startPos);
    }

    return result;
}

int main()
{
    int titleSize = 6;
    string s = "intitle:bchd hdsjv hvwuh jvn";
    vector<string> t = {"bchd", "vjnd", "vjnd", "bchd", "hdj", "hvwuh"};
    s.erase(s.begin(), s.begin() + 8);
    cout << s << endl;
    vector<string> v = splitPharse(s);
    for (auto z : v)
        cout << z << " " << count(t.begin(), t.begin() + titleSize, z) << endl;
    return 0;
=======
#include "screen.h"
#include "read.h"

int main() {
    scanStopword();
    TRIE Trie;
    vector <string> fileName;
    vector <vector<string>> fileData;
    importFile(Trie, "Search Engine-Data", fileName, fileData);
    SYNONYM_DATA synonymData;
    synonymData.importData();
    searchBox(Trie, synonymData, fileData, fileName);
    // searchBox();
    return 0;
}

/*#include <iostream>
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

vector<string> splitPharse(const string &inputStr)
{
    vector<string> result;
    size_t startPos = 0, endPos = inputStr.find_first_of(' ');

    while (endPos <= string::npos)
    {
        string tempStr = inputStr.substr(startPos, endPos - startPos);
        transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
        result.emplace_back(tempStr);

        if (endPos == string::npos)
            return result;

        startPos = endPos + 1;
        endPos = inputStr.find_first_of(' ', startPos);
    }

    return result;
}

void print5BestResult(RESULT_MAP &resultMap, vector<vector<string>> &fileData)
{
    vector<RESULT_PAIR> resultVector;
    for (RESULT_PAIR resultPair : resultMap)
        resultVector.push_back(resultPair);

    sort(resultVector.begin(), resultVector.end(), isBetterResult);

    for (int i = 0; i < 5; ++i)
    {
        int fileID = resultVector[i].first;

        cout << "File " << fileID << ":\n| ";
        for (pair<string, int> wordInfo : resultVector[i].second)
            cout << wordInfo.first << ": " << wordInfo.second << " | ";
        cout << endl;

        vector<string> yeu_and_Dung_rat_nhieu = fileData[fileID];

        for (pair<string, int> Dung_gay : resultVector[i].second)
            if (Dung_gay.first.find_first_of(" ") == string::npos)
            {
                vector<string>::iterator tim_thay_roi = find(yeu_and_Dung_rat_nhieu.begin(), yeu_and_Dung_rat_nhieu.end(), Dung_gay.first);
            }
            else
            {
                vector<string> toi_da_tram_cam = splitPharse(Dung_gay.first);
                vector<string>::iterator tim_thay_roi = find(yeu_and_Dung_rat_nhieu.begin(), yeu_and_Dung_rat_nhieu.end(), toi_da_tram_cam[0]);
            }
    }
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

bool isAlphaOrDigit(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '0');
}

bool isOperator(const string &word)
{
    return word == "AND" || word == "OR";
}

bool isStartWiths(const string &word, const string &chars)
{
    return word.substr(0, chars.length()) == chars;
}

bool hasOperator(const string &word)
{
    if (isStartWiths(word, "intitle:") || isStartWiths(word, "filetype"))
        return true;

    vector<char> oprts = {'+', '-', '$', '#', '\"'};
    for (char oprt : oprts)
        if (word[0] == oprt)
            return true;

    if (word.find_first_of("..") != string::npos || word.find_first_of("*") != string::npos)
        return true;
    return false;
}

vector<string> splitInput(const string &inputStr)
{
    vector<string> result;
    size_t startPos = 0, endPos = inputStr.find_first_of(' ');

    while (endPos <= string::npos)
    {
        if (inputStr[startPos] == '\"')
        {
            endPos = inputStr.find_first_of('\"', startPos + 1);
            result.emplace_back(inputStr.substr(startPos, endPos - startPos + 1));
            if (endPos == string::npos)
            {
                return result;
            }
            startPos = endPos + 1;
            endPos = inputStr.find_first_of(' ', startPos);
            continue;
        }

        string tempStr = inputStr.substr(startPos, endPos - startPos);

        if (tempStr == "AND" || tempStr == "OR")
        {
            result.emplace_back(tempStr);
        }

        else if (tempStr == "*")
        {
            while (!result.empty())
            {
                string tempWord = result[result.size() - 1];

                if (!isOperator(tempWord) && !hasOperator(tempWord))
                {
                    tempStr = tempWord + " " + tempStr;
                    result.pop_back();
                }
                else
                    break;
            }

            startPos = endPos + 1;
            endPos = inputStr.find_first_of(' ', startPos);

            while (endPos <= string::npos)
            {
                string tempTempStr = inputStr.substr(startPos, endPos - startPos);

                if (!isOperator(tempTempStr) && !hasOperator(tempTempStr))
                    tempStr += " " + tempTempStr;
                else
                {
                    startPos -= tempTempStr.length() + 1;
                    endPos -= tempTempStr.length() + 1;
                    break;
                }

                if (endPos == string::npos)
                {
                    result.emplace_back(tempStr);
                    for (int i = 0; i < result.size(); ++i)
                        for (int j = 0; j < result[i].length(); ++j)
                            if (result[i][j] == ' ')
                                result[i][j] = '1';
                    return result;
                }

                startPos = endPos + 1;
                endPos = inputStr.find_first_of(' ', startPos);
            }

            result.emplace_back(tempStr);
        }

        else if (isStartWiths(tempStr, "intitle:"))
        {
            if (endPos == string::npos)
            {
                result.emplace_back(tempStr);

                return result;
            }
            startPos = endPos + 1;
            endPos = inputStr.find_first_of(' ', startPos);

            while (endPos <= string::npos)
            {
                string tempTempStr = inputStr.substr(startPos, endPos - startPos);

                if (!isOperator(tempTempStr) && !hasOperator(tempTempStr))
                {
                    cout << "yes\n";
                    tempStr += " " + tempTempStr;
                }
                else
                {
                    startPos -= tempTempStr.length() + 1;
                    endPos -= tempTempStr.length() + 1;
                    break;
                }
                //cout<<tempStr<<endl;
                if (endPos == string::npos)
                {
                    result.emplace_back(tempStr);

                    return result;
                }

                startPos = endPos + 1;
                endPos = inputStr.find_first_of(' ', startPos);
            }

            result.emplace_back(tempStr);
        }

        else if (tempStr != " " && tempStr != "")
        {
            transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
            result.emplace_back(tempStr);
        }

        if (endPos == string::npos)
            return result;

        startPos = endPos + 1;
        endPos = inputStr.find_first_of(' ', startPos);
    }

    return result;
}

int main()
{
    int titleSize = 6;
    string s = "intitle:bchd hdsjv hvwuh jvn";
    vector<string> t = {"bchd", "vjnd", "vjnd", "bchd", "hdj", "hvwuh"};
    s.erase(s.begin(), s.begin() + 8);
    cout << s << endl;
    vector<string> v = splitPharse(s);
    for (auto z : v)
        cout << z << " " << count(t.begin(), t.begin() + titleSize, z) << endl;
    return 0;
>>>>>>> 7a5cba0b462b8bfab459e09fe3e83d2f40695a2e
}*/