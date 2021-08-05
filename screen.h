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
using namespace std;

COORD cursorPosition;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoXY(int x, int y) {
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

int getKey() {
    int z; z = _getch();
    if (z == 224) {
        char c = _getch();
        if (c == 72) return UP;
		if (c == 80) return DOWN;
		if (c == 77) return RIGHT;
		if (c == 75) return LEFT;
    }
    return z;
}

void searchBox() {
    ofstream historyFile("search_history.txt", ios::app);

    TRIE_SEARCHING_HISTORY Trie;
    scanSearchHistory(Trie);

    system ("cls");
    // gotoXY(0, 0); cout << "--- Searching Engine ---";
    // gotoXY(0, 1); cout << "To escape, press ESC.\n";
    // system("pause");
    // system("cls");

    string curSearch;
    int curLine = -1;
    while (true) {
        gotoXY(0, 4); cout << "INPUT: " << curSearch;
        auto suggest = Trie.search(curSearch);
        if (curSearch.empty()) suggest.clear();
        for (int i = 0; i < suggest.size(); i++) {
            gotoXY(7, 5+i); cout << i + 1 << ". " << suggest[i];
        }
        gotoXY(7 + curSearch.size(), 4);
        int key = getKey();
        if (key == UP) {
            if  (!(curLine > 0)) continue;
            gotoXY(0, 5+curLine); 
            cout << "       ";
            curLine--;
            gotoXY(0, 5+curLine);
            cout << "  ||   ";
        }
        else if (key == DOWN) {
            if (!(curLine < (int) suggest.size() - 1)) continue;
            gotoXY(0, 5+curLine);
            cout << "       ";
            curLine++;
            gotoXY(0, 5+curLine);
            cout << "  ||   ";
        }
        else if (key == TAB) {
            if (!(curLine >= 0)) continue;
            curSearch = suggest[curLine];
            system("cls");
            gotoXY(0, 5+curLine);
            cout << "       ";
            curLine = -1;
        }
        else if (key == ESC) {
            system("cls");
            break;
        }
        else if (key == ENTER) {
            system("cls");
            // searching(curSearch);
            cout << " Searching ... " << curSearch << '\n';
            Trie.insert(curSearch);
            historyFile << curSearch << '\n';
            system ("pause");
            system("cls");
        }
        else if (key == BACKSPACE) {
            curSearch.pop_back();
            system("cls");
            curLine = -1;
        }
        else {
            curSearch += char(key);
            system ("cls");
            if (curLine != -1) {
                gotoXY(0, 5+curLine);
                cout << "       ";
            }
            curLine = -1;   
        }
    }
}

#endif