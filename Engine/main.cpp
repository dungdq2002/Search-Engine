#include "screen.h"
#include "read.h"

int main() {
    scanStopword();

    SYNONYM_DATA synonymData;
    synonymData.importData();
    
    TRIE Trie;
    vector <string> fileName;
    vector <vector<string>> fileData;

    cout << "Importing....\n";
    auto start = high_resolution_clock::now();

    fileName = loadFileName();
    loadData(Trie, fileData);

    string folder = "small-data";
    auto newFile = findNewFile(fileName, folder);

    importFile(Trie, folder, newFile, fileName, fileData);

    auto finish = high_resolution_clock::now();
    auto duration = duration_cast <milliseconds>(finish - start);

    if (newFile.size()) {
        saveFileName(fileName);
        saveDataToFile(Trie, fileData); 
    }
    
    cout << "Finished in " << duration.count() << " ms.\n";

    system("pause");

    searchBox(Trie, synonymData, fileData, fileName, folder);
    
    return 0;
}
