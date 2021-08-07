#ifndef READ_H
#define READ_H

#include "trie.h"
#include "normalize.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

struct SYNONYM_DATA
{
    unordered_map<string, int> idDict;
    vector<vector<string>> dict;
    SYNONYM_DATA()
    {
        read_file();
    }

    void read_file()
    {
        fstream fin;

        fin.open("data.csv", ios::in);

        vector<string> row;
        string line, word, temp;

        int rowID = 0, tmp;

        while (fin >> temp) {

            row.clear();

            getline(fin, line);

            stringstream s(line);

            tmp = 0;
            while (getline(s, word, ',')) {
                if (tmp > 0)
                {
                    row.push_back(word);
                    idDict[word] = rowID;
                }
                tmp++;
            }

            dict.push_back(row);

            rowID++;
            if (rowID == 10)
                break;
        }
    }
};

void importFile(TRIE &Trie, string folder, vector<string> &fileName, vector<vector<string>> &fileData)
{
    auto path = fs::current_path();
    path += "\\" + folder;
    for (const auto &file : fs::directory_iterator(path))
    {
        // File name
        string _fileName = fs::path(file).filename().string();
        fileName.push_back(_fileName);

        // Scan file data
        ifstream inp(file.path());
        string s;
        vector<string> _fileData;
        while (inp >> s)
        {
            clean(s);
            if (s.size())
            {
                Trie.insert(s, fileName.size() - 1);
                _fileData.push_back(s);
            }
        }
        fileData.push_back(_fileData);
    }
}

void scanSearchHistory(TRIE_SEARCHING_HISTORY &Trie) {
    ifstream inp("search_history.txt");
    cerr << "hello\n";
    for (string s; getline(inp, s); ) {
        cerr << "hello - " << s << '\n';
        Trie.insert(s);
        cerr << "done " << s << '\n';
    }
    // cerr << "done all!";
}

#endif