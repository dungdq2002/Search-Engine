#include "screen.h"
#include "read.h"

int main() {
    scanStopword();
    TRIE Trie;
    vector <string> fileName;
    vector <vector<string>> fileData;
    importFile(Trie, "sample-data", fileName, fileData);
    SYNONYM_DATA synonymData;
    synonymData.read_file();
    string word = "get";
    cout << "size " << synonymData.dict[synonymData.idDict[word]].size() << '\n';
    for (auto it: synonymData.dict[synonymData.idDict[word]]) {
        cout << it << '\n';
    }
    exit(0);
    searchBox(Trie, synonymData, fileData);
    // searchBox();
    return 0;
}