#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include "read.h"
#include "operator.h"
#include "normalize.h"
using namespace std;

COORD cursorPosition;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoXY(int x, int y)
{
    cursorPosition.X = x;
    cursorPosition.Y = y;
    SetConsoleCursorPosition(console, cursorPosition);
}

#define UP -1
#define DOWN -2
#define RIGHT -3
#define LEFT -4
#define ESC 27
#define ENTER 13
#define BACKSPACE 8
#define TAB 9

int getKey()
{
    int z;
    z = _getch();
    if (z == 224)
    {
        char c = _getch();
        if (c == 72)
            return UP;
        if (c == 80)
            return DOWN;
        if (c == 77)
            return RIGHT;
        if (c == 75)
            return LEFT;
    }
    return z;
}

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

void print5BestResult_highlight(RESULT_MAP &resultMap, vector<vector<string>> &fileData)
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

void searchBox(TRIE &trie, SYNONYM_DATA &synonymData, vector<vector<string>> &fileData, vector<string> &fileName)
// void searchBox()
{
    ofstream historyFile("Resources/search_history.txt", ios::app);

    TRIE_SEARCHING_HISTORY Trie;
    scanSearchHistory(Trie);

    system("cls");
    // gotoXY(0, 0); cout << "--- Searching Engine ---";
    // gotoXY(0, 1); cout << "To escape, press ESC.\n";
    // system("pause");
    // system("cls");

    string curSearch;
    int curLine = -1;
    while (true)
    {
        gotoXY(0, 4);
        cout << "INPUT: " << curSearch;
        auto suggest = Trie.search(curSearch);
        if (curSearch.empty())
            suggest.clear();
        for (int i = 0; i < suggest.size(); i++)
        {
            gotoXY(7, 5 + i);
            cout << i + 1 << ". " << suggest[i];
        }
        gotoXY(7 + curSearch.size(), 4);
        int key = getKey();
        if (key == UP)
        {
            if (!(curLine > 0))
                continue;
            gotoXY(0, 5 + curLine);
            cout << "       ";
            curLine--;
            gotoXY(0, 5 + curLine);
            cout << "  ||   ";
        }
        else if (key == DOWN)
        {
            if (!(curLine < (int)suggest.size() - 1))
                continue;
            gotoXY(0, 5 + curLine);
            cout << "       ";
            curLine++;
            gotoXY(0, 5 + curLine);
            cout << "  ||   ";
        }
        else if (key == TAB)
        {
            if (!(curLine >= 0))
                continue;
            curSearch = suggest[curLine];
            system("cls");
            gotoXY(0, 5 + curLine);
            cout << "       ";
            curLine = -1;
        }
        else if (key == ESC)
        {
            system("cls");
            break;
        }
        else if (key == ENTER)
        {
            system("cls");
            cout << " Searching ... " << curSearch << '\n';
            auto res = handleInput(curSearch, trie, synonymData, fileData, fileName);
            print5BestResult_highlight(res, fileData);
            // searching(curSearch);
            Trie.insert(curSearch);
            historyFile << curSearch << '\n';
            system("pause");
            system("cls");
        }
        else if (key == BACKSPACE)
        {
            curSearch.pop_back();
            system("cls");
            curLine = -1;
        }
        else
        {
            curSearch += char(key);
            system("cls");
            if (curLine != -1)
            {
                gotoXY(0, 5 + curLine);
                cout << "       ";
            }
            curLine = -1;
        }
    }
}

#endif