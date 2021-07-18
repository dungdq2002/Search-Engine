#ifndef NORMALIZE_H
#define NORMALIZE_H

/*#include <string>
#include <cctype>
#include <fstream>
#include <iostream>
#include <unordered_set>
using namespace std;*/
#include "library.h"

unordered_set<string> __stopword__;

void scanStopword()
{
    ifstream inp("stopword.txt");
    for (string s; inp >> s;)
        __stopword__.insert(s);
}

bool trueChar(char c)
{
    if (c == '-' || c == '$' || c == '#')
        return true;
    if (isalnum(c))
        return true;
    return false;
}

void clean(string &s)
{
    string t, NONE;
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

#endif