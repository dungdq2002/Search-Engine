#ifndef READ_H
#define READ_H

/*#include "trie.h"
#include "normalize.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;*/
#include "library.h"

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

#endif