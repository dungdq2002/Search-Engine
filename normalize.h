#ifndef NORMALIZE_H
#define NORMALIZE_H

#include <string>
#include <cctype>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>
using namespace std;

unordered_set<string> __stopword__;

void scanStopword()
{
    ifstream inp("stopword.txt");
    for (string s; inp >> s;)
        __stopword__.insert(s);
}

bool trueChar(char c)
{
    if (c == '-' || c == '$' || c == '#' || c == '~')
        return true;
    if (isalnum(c))
        return true;
    return false;
}

void clean(string &s)
{
    string t = "", NONE = "";
    for (char c : s)
    {
        if (c == -30)
            break;
        if (!trueChar(c))
        {
            s = NONE;
            return;
        };
        c = towlower(c);
        t += c;
    }
    s = t;
    if (__stopword__.find(s) != __stopword__.end())
        s = NONE;
}

/*pair<int, int> getOperator(string &inputStr)
{
    if (inputStr[0] == '\"')
    {
        inputStr.erase(inputStr.begin());
        inputStr.pop_back();
        return make_pair(11, 0);
    }
    vector<string> oprts = {
                           
                            "*"};//
    for (int i = 0; i < 11; ++i)
    {
        size_t find = inputStr.find(oprts[i]);
        if (find != string::npos)
        {
            inputStr.erase(find, find + oprts[i].length());
            return make_pair(i, find);
        }
    }
    return make_pair(-1, 0);
}*/

vector<string> splitInput(const string &inputStr)
{
    vector<string> result;
    size_t startPos = 0, endPos = inputStr.find_first_of(' ');

    while (endPos <= string::npos)
    {
        string tempStr = inputStr.substr(startPos, endPos - startPos);

        //transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
        result.emplace_back(tempStr);

        if (endPos == string::npos)
            break;
        startPos = endPos + 1;
        endPos = inputStr.find_first_of(' ', startPos);
    }

    return result;
}

pair<int, int> getOperator(vector<string> &keyWords)
{
    //  " "
    if (keyWords[0][0] == '\"')
    {
        keyWords[0].erase(keyWords[0].begin());
        keyWords[keyWords.size() - 1].pop_back();
        return make_pair(10, 0);
    }
    vector<string> oprts = {"AND", "OR", "*", "intitle:", "filetype:", "#", "~", "+", "-", "$"};

    //  AND, OR, *
    for (int i = 0; i < 3; ++i)
    {
        vector<string>::iterator found = find(keyWords.begin(), keyWords.end(), oprts[i]);
        if (found != keyWords.end())
        {
            keyWords.erase(found);
            return make_pair(i, found - keyWords.begin());
        }
    }

    //  intitle:, filetype:, #, ~
    for (int i = 3; i < 7; ++i)
    {
        if (keyWords[0].find(oprts[i]) == 0)
        {
            keyWords[0].erase(0, oprts[i].length());
            return make_pair(i, 0);
        }
    }

    //  +, -, $
    for (int i = 7; i < 10; ++i)
    {
        for (int j = 0; j < keyWords.size(); ++j)
        {
            if (keyWords[j][0] == oprts[i][0])
            {
                keyWords[j].erase(keyWords[j].begin());
                return make_pair(i, j);
            }
        }
    }
    return make_pair(-1, 0);
}

void normalizeKeyWords(vector<string> &keyWords)
{
    for (int i = 0; i < keyWords.size(); ++i)
    {
        transform(keyWords[i].begin(), keyWords[i].end(), keyWords[i].begin(), ::tolower);
        if (__stopword__.find(keyWords[i]) != __stopword__.end())
            keyWords.erase(keyWords.begin() + i);
    }
}
#endif