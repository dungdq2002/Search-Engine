#ifndef READ_H
#define READ_H

#include "trie.h"
#include "normalize.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <chrono>
using namespace std;
using namespace std::chrono;
namespace fs = filesystem;

struct SYNONYM_DATA
{
    unordered_map<string, int> idDict;
    vector<vector<string>> dict;

    void splitSynonym(const string &inputStr, int indexInDict)
    {
        vector<string> result;
        size_t startPos = 0, endPos = inputStr.find_first_of(',');
        while (endPos <= string::npos)
        {
            string tempStr = inputStr.substr(startPos, endPos - startPos);
            transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);

            idDict[tempStr] = indexInDict;
            result.push_back(tempStr);

            if (endPos == string::npos)
            {
                dict.push_back(result);
                return;
            }

            startPos = endPos + 1;
            endPos = inputStr.find_first_of(',', startPos);
        }
    }

    void importData()
    {
        dict.push_back({});

        ifstream fin;
        fin.open("Resources/Synonym.csv");
        if (fin.is_open())
        {
            int index = 0;

            while (!fin.eof())
            {
                ++index;

                string synonym;
                fin >> synonym;

                splitSynonym(synonym, index);
            }
        }
        fin.close();
    }
};

void importFile(TRIE &Trie, string folder, vector<string> &fileName, vector<vector<string>> &fileData)
{
    auto path = fs::current_path();
    path += "\\" + folder;
    auto start = high_resolution_clock::now();
    int t = 0;
    int tmp1 = 2818, tmp2 = 5636, tmp3 = 8454, tmp4 = 11271;
    for (const auto &file : fs::directory_iterator(path))
    {
        t++;
        auto stop = high_resolution_clock::now();
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
        if (t == tmp1) {
            auto duration = duration_cast<milliseconds> (stop - start);
            cout << "Phase 1: " << tmp1 << " files -> " << duration.count() << " ms.\n";
        }
        if (t == tmp2) {
            auto duration = duration_cast<milliseconds> (stop - start);
            cout << "Phase 2: " << tmp2 << " files -> " << duration.count() << " ms.\n";
        }
        if (t == tmp3) {
            auto duration = duration_cast<milliseconds> (stop - start);
            cout << "Phase 3: " << tmp3 << " files -> " << duration.count() << " ms.\n";
        }
        if (t == tmp4) {
            auto duration = duration_cast<milliseconds> (stop - start);
            cout << "Phase 4: " << tmp4 << " files -> " << duration.count() << " ms.\n";
        }
    }
}

void scanSearchHistory(TRIE_SEARCHING_HISTORY &Trie)
{
    ifstream inp("Resources/search_history.txt");
    for (string s; getline(inp, s);)
    {
        Trie.insert(s);
    }
    // cerr << "done all!";
}

#endif