#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include "read.h"
#include "operator.h"
#include "normalize.h"
using namespace std;
using namespace std::chrono;

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

void printSpecial(string s)
{
    SetConsoleTextAttribute(console, 9);
    cout << s << ' ';
    SetConsoleTextAttribute(console, 15);
}

void print5BestResult_highlight(RESULT_MAP &resultMap, vector<vector<string>> &fileData, vector<string> &fileName)
{
    vector<RESULT_PAIR> resultVector;
    for (RESULT_PAIR resultPair : resultMap)
        resultVector.push_back(resultPair);

    if (resultVector.empty())
    {
        cout << "Not found!\n";
        return;
    }

    sort(resultVector.begin(), resultVector.end(), isBetterResult);

    cout << '\n';

    for (int i = 0; i < min(int(resultVector.size()), 5); ++i)
    {
        int fileID = resultVector[i].first;

        cout << "File " << fileID << ": " << fileName[fileID] << '\n';
        cout << "| ";
        for (pair<string, int> wordInfo : resultVector[i].second)
            cout << wordInfo.first << ": " << wordInfo.second << " | ";
        cout << endl;

        vector<string> em_dang_cam_data_ne = fileData[fileID];
        // A * C
        // A B C
        // A B C
        unordered_map<string, bool> huhu;

        bool truong_hop_db = false;
        vector<string> bugdemkhuya;

        for (pair<string, int> Dung_gay : resultVector[i].second)
            if (Dung_gay.first.find_first_of(" ") == string::npos)
            {
                // vector<string>::iterator tim_thay_roi = find(em_dang_cam_data_ne.begin(), em_dang_cam_data_ne.end(), Dung_gay.first);
                // if (tim_thay_roi == em_dang_cam_data_ne.begin()) {
                //     SetConsoleTextAttribute(console, 9);
                //     cout <<
                // }
                huhu[Dung_gay.first] = true;
            }
            else
            {
                vector<string> toi_da_tram_cam = splitPharse(Dung_gay.first);
                // vector<string>::iterator tim_thay_roi = find(em_dang_cam_data_ne.begin(), em_dang_cam_data_ne.end(), toi_da_tram_cam[0]);
                for (auto it : toi_da_tram_cam)
                    huhu[it] = true;
                bugdemkhuya = toi_da_tram_cam;
                truong_hop_db = true;
            }

        auto tao_da_tim_ra = em_dang_cam_data_ne.end();
        if (truong_hop_db)
        {
            auto tao_di_tim = find(em_dang_cam_data_ne.begin(), em_dang_cam_data_ne.end(), bugdemkhuya[0]);

            while (tao_di_tim != em_dang_cam_data_ne.end())
            {
                bool isExact = true;

                for (int i = 0; i < bugdemkhuya.size(); ++i)
                {
                    if (tao_di_tim + i == bugdemkhuya.end())
                    {
                        isExact = false;
                        break;
                    }

                    if (*(tao_di_tim + i) != bugdemkhuya[i])
                    {
                        // cout << *(tao_di_tim + i) << ' ' << bugdemkhuya[i] << '\n';
                        isExact = false;
                        break;
                    }
                }

                // cout << tao_di_tim - em_dang_cam_data_ne.begin() << '\n';

                if (isExact)
                {
                    tao_da_tim_ra = tao_di_tim;
                    break;
                }

                // cout << "im here " << isExact << '\n';
                tao_di_tim = find(tao_di_tim + 1, em_dang_cam_data_ne.end(), bugdemkhuya[0]);
                // cout << tao_di_tim - em_dang_cam_data_ne.begin() << '\n';
            }

            if (tao_di_tim != em_dang_cam_data_ne.end())
            {
                if (tao_da_tim_ra == em_dang_cam_data_ne.begin())
                {
                    printSpecial(*tao_da_tim_ra);
                    for (auto cai_gi_vay_troi = tao_da_tim_ra + 1; cai_gi_vay_troi < tao_da_tim_ra + 7 && cai_gi_vay_troi < em_dang_cam_data_ne.end(); cai_gi_vay_troi++)
                    {
                        if (!huhu[*cai_gi_vay_troi])
                            cout << *cai_gi_vay_troi << ' ';
                        else
                            printSpecial(*cai_gi_vay_troi);
                    }
                    cout << "...";
                }
                else
                {
                    cout << "... ";
                    for (auto cai_gi_vay_troi = tao_da_tim_ra - min(tao_da_tim_ra - em_dang_cam_data_ne.begin(), 6); cai_gi_vay_troi < tao_da_tim_ra; cai_gi_vay_troi++)
                    {
                        if (!huhu[*cai_gi_vay_troi])
                            cout << *cai_gi_vay_troi << ' ';
                        else
                            printSpecial(*cai_gi_vay_troi);
                    }
                    printSpecial(*tao_da_tim_ra);
                    for (auto cai_gi_vay_troi = tao_da_tim_ra + 1; cai_gi_vay_troi < tao_da_tim_ra + 7 && cai_gi_vay_troi < em_dang_cam_data_ne.end(); cai_gi_vay_troi++)
                    {
                        if (!huhu[*cai_gi_vay_troi])
                            cout << *cai_gi_vay_troi << ' ';
                        else
                            printSpecial(*cai_gi_vay_troi);
                    }
                    cout << "... ";
                }
            }
        }

        if (tao_da_tim_ra == em_dang_cam_data_ne.end())
        {

            for (auto it : huhu)
            {
                vector<string>::iterator tim_thay_roi = find(em_dang_cam_data_ne.begin(), em_dang_cam_data_ne.end(), it.first);
                if (tim_thay_roi == em_dang_cam_data_ne.begin())
                {
                    printSpecial(it.first);
                    for (auto cai_gi_vay_troi = tim_thay_roi + 1; cai_gi_vay_troi < tim_thay_roi + 7 && cai_gi_vay_troi < em_dang_cam_data_ne.end(); cai_gi_vay_troi++)
                    {
                        if (!huhu[*cai_gi_vay_troi])
                            cout << *cai_gi_vay_troi << ' ';
                        else
                            printSpecial(*cai_gi_vay_troi);
                    }
                    cout << "...";
                }
                else
                {
                    cout << "... ";
                    for (auto cai_gi_vay_troi = tim_thay_roi - min(tim_thay_roi - em_dang_cam_data_ne.begin(), 6); cai_gi_vay_troi < tim_thay_roi; cai_gi_vay_troi++)
                    {
                        if (!huhu[*cai_gi_vay_troi])
                            cout << *cai_gi_vay_troi << ' ';
                        else
                            printSpecial(*cai_gi_vay_troi);
                    }
                    printSpecial(it.first);
                    for (auto cai_gi_vay_troi = tim_thay_roi + 1; cai_gi_vay_troi < tim_thay_roi + 7 && cai_gi_vay_troi < em_dang_cam_data_ne.end(); cai_gi_vay_troi++)
                    {
                        if (!huhu[*cai_gi_vay_troi])
                            cout << *cai_gi_vay_troi << ' ';
                        else
                            printSpecial(*cai_gi_vay_troi);
                    }
                    cout << "... ";
                }
                break;
            }
        }

        cout << '\n';
        cout << "----------------------------------------------------\n\n";
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
            Trie.insert(curSearch);
            historyFile << curSearch << '\n';
            auto start = high_resolution_clock::now();
            auto res = handleInput(curSearch, trie, synonymData, fileData, fileName);
            auto finish = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(finish - start);
            cout << " Found " << 
            cout << duration.count() << " microseconds.\n";
            // exit(0);
            print5BestResult_highlight(res, fileData, fileName);
            // searching(curSearch);
            system("pause");
            system("cls");
        }
        else if (key == BACKSPACE)
        {
            if (curSearch.size())
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