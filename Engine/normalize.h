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

void scanStopword()
{
    ifstream inp("Resources/stopword.txt");
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
                return result;
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
                    return result;
                }

                startPos = endPos + 1;
                endPos = inputStr.find_first_of(' ', startPos);
            }

            result.emplace_back(tempStr);
        }

        else if (isStartWiths(tempStr, "intitle:"))
        {
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
                    return result;
                }

                startPos = endPos + 1;
                endPos = inputStr.find_first_of(' ', startPos);
            }

            result.emplace_back(tempStr);
        }

        else if (tempStr != " ")
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

#endif