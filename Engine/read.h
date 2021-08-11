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
using namespace std;
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

void scanSearchHistory(TRIE_SEARCHING_HISTORY &Trie)
{
    ifstream inp("Resources/search_history.txt");
    cerr << "hello\n";
    for (string s; getline(inp, s);)
    {
        cerr << "hello - " << s << '\n';
        Trie.insert(s);
        cerr << "done " << s << '\n';
    }
    // cerr << "done all!";
}

#endif