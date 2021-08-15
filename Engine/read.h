#ifndef READ_H
#define READ_H

#include "trie.h"
#include "normalize.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
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

void importFile(TRIE &Trie, string folder, vector <string> newFile, vector<string> &fileName, vector<vector<string>> &fileData)
{
    auto path = fs::current_path();
    path += "\\" + folder;
    // auto start = high_resolution_clock::now();
    // int t = 0;
    // int tmp1 = 2818, tmp2 = 5636, tmp3 = 8454, tmp4 = 11271;
    
    for (string _fileName: newFile)
    {
        auto file = path;
        file += "\\" + _fileName;
        // t++;
        // auto stop = high_resolution_clock::now();
        // File name
        fileName.push_back(_fileName);

        // Scan file data
        ifstream inp(file.string());
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
        // if (t == tmp1) {
        //     auto duration = duration_cast<milliseconds> (stop - start);
        //     cout << "Phase 1: " << tmp1 << " files -> " << duration.count() << " ms.\n";
        // }
        // if (t == tmp2) {
        //     auto duration = duration_cast<milliseconds> (stop - start);
        //     cout << "Phase 2: " << tmp2 << " files -> " << duration.count() << " ms.\n";
        // }
        // if (t == tmp3) {
        //     auto duration = duration_cast<milliseconds> (stop - start);
        //     cout << "Phase 3: " << tmp3 << " files -> " << duration.count() << " ms.\n";
        // }
        // if (t == tmp4) {
        //     auto duration = duration_cast<milliseconds> (stop - start);
        //     cout << "Phase 4: " << tmp4 << " files -> " << duration.count() << " ms.\n";
        // }
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

void saveFileName(vector<string> &fileName)
{
    ofstream fout;
    fout.open("Resources/file_name.txt");
    if (fout.is_open())
    {
        for (string name : fileName)
            fout << name << endl;
    }
    else
        cout << "Can't open file file_name.txt\n";
}

vector<string> loadFileName()
{
    vector<string> result;
    ifstream fin;
    fin.open("Resources/file_name.txt");
    if (fin.is_open())
    {
        string name;
        while (!fin.eof())
        {
            getline(fin, name);
            if (name.empty()) break;
            result.push_back(name);
        }
    }
    return result;
}

void saveDataToFile(TRIE &Trie, vector < vector <string> > &fileData) {
    ofstream dataTrie("Resources/save_data.txt");
    Trie.save(dataTrie);
    dataTrie << "=====\n";
    for (auto vec : fileData) {
        for (auto str: vec) dataTrie << str << ' ';
        dataTrie << '\n';
    }
}

void loadData(TRIE &Trie, vector < vector < string> > &fileData) {
    ifstream inp("Resources/save_data.txt");
    if (!inp) return;
    for (string s; inp >> s; ) {
        if (s == "=====") {
            getline(inp, s);
            break;
        }
        int sz; inp >> sz;
        unordered_map <int, int> um;
        for (int i = 1; i <= sz; i++) {
            int id, num;
            inp >> id >> num;
            um[id] = num;
        }
        Trie.insert(s, um);
    }
    for (string s; getline(inp, s); ) {
        if (s.empty()) break;
        stringstream ss(s);
        string t;
        vector <string> vec;
        bool first = true;
        while (ss >> t) {
            vec.push_back(t);
        }
        fileData.push_back(vec);
    }
}

vector<string> findNewFile(vector<string> &fileName, string path)
{
    unordered_set <string> Set(fileName.begin(), fileName.end());
    vector<string> res;
    for (const auto &entry : fs::directory_iterator(path))
    {
        string s = entry.path().string();
        s.erase(s.begin(), s.begin() + path.length() + 1);
        if (Set.find(s) == Set.end())
            res.push_back(s);
    }
    return res;
}

#endif