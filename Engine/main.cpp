#include "screen.h"
#include "read.h"

int main() {
    scanStopword();
    TRIE Trie;
    vector <string> fileName;
    vector <vector<string>> fileData;
    cout << "Importing....\n";
    auto start = high_resolution_clock::now();
    importFile(Trie, "large-data", fileName, fileData);
    auto finish = high_resolution_clock::now();
    auto duration = duration_cast <milliseconds>(finish - start);
    SYNONYM_DATA synonymData;
    synonymData.importData();
    cout << "Finished in " << duration.count() << " ms.\n";
    system("pause");
    searchBox(Trie, synonymData, fileData, fileName);
    return 0;
}
