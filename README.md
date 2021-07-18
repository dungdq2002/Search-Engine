# Search Engine - CS163

[Link github](https://github.com/leviosarz/Search-Engine-CS163)

**Tất cả làm bằng visual studio code, c++17, hệ điều hành windows.**

- Các đoạn text sẽ được loại hết dấu câu, đưa về lowercase và loại hết stop word. Hai kí tự ‘#’ và ‘$’ sẽ được giữ lại.

- Một vài biến sẽ sử dụng:
fileName[]: fileName[i] là tên của file thứ i, ví dụ fileName[1] = “1.txt”.
vector <string> fileData[]: fileData[i] là dữ liệu của file thứ i.

- Các đoạn text sẽ được thêm vào cây Trie, từng word một. Ở node cuối cùng của một từ, sẽ có một unordered_map <int, int> lưu lại, với key là id file và value là số lần xuất hiện của từ đó trong file. Đối với số, sẽ đặt những số 0 ở đầu số, để mỗi số đủ 10 kí tự, giúp thuận tiện cho thao tác tìm range.
Ví dụ: Với 3 đoạn text là “1.txt” : “Search engine”, “2.txt” : “engine vs engine in england”, “3.txt” : “#abc $99 123456789”, [cây Trie sẽ có dạng sau.](https://studenthcmusedu-my.sharepoint.com/:i:/g/personal/20125127_student_hcmus_edu_vn/EamnA0oCfpFDsLKpZr9cbSMB84PErnz8LXeD_pMI-d4YFA?e=Wf3Dga)

- Để kiểm tra một key ở text 1 có xuất hiện ở text 2 hay không, [có thể làm theo như này, không làm O(N^2).](https://ideone.com/3a4AID)

- Nếu chỉ là một đoạn text thông thường, ví dụ 'Search engine' thì sẽ được xử lý là "Search" OR "Engine". Độ ưu tiên bằng tổng số lần xuất hiện của cả 2 từ.

- Các operator sẽ được xử lý từ trái sang phải.

# Operators:

:coffee: AND: Truy vấn loại này sẽ được xử lý: <Kết quả tìm được bên trái> AND <Kết quả tìm được bên phải>, tức cả 2 bên trái và phải sẽ tìm được trong đoạn text.
Cách tìm: Sau khi tìm được những file phù hợp bên trái và file phù hợp bên phải, hợp 2 tập hợp này lại và tìm tập hợp con. Độ ưu tiên bằng tổng số lần xuất hiện của cả trái và phải

:coffee: OR: Tương tự như search thông thường.

:coffee: “-” operator: Truy vấn loại này là tìm đoạn text không chứa từ sau dấu ‘-’, để thực hiện thì ta sẽ tìm hết những file chứa các word còn lại, sau đó loại đi những file có word của từ sau dấu ‘-’.

:coffee: intitle:hammer nails: Truy vấn loại này, sau khi đã tìm được toàn bộ text chứa word đằng sau, ta sẽ lọc ra những text có tên là “hammer”. Lưu ý ở đây, mình chỉ xử lý cho “hammer” trong title, chưa tính đến xử lý nguyên đoạn “hammer nails” trong title.

:coffee: “+” operator: làm giống truy vấn AND

:coffee: filetype:txt: giống intitle

:coffee: Search for a price. Put $ in front of a number. For example: camera $400, tìm như thông thường.

:coffee: Search hashtags. Put # in front of a word. For example: #throwbackthursday, tìm như thông thường.

:coffee: Search for an exact match. Put a word or phrase inside quotes. For example, "tallest building". Lọc một filter lượt một là các đoạn text chứa tallest AND building, sau đó for qua đoạn text để đếm xem số lần “tallest building” xuất hiện.

:coffee: Search for wildcards or unknown words. Put a * in your word or phrase where you want to leave a placeholder. For example, "largest * in the world". Cũng lọc một filter “largest” AND “in the world”. Lưu ý ở đây, cách lấy word của từ tìm kiếm khác với cách lấy thông thường. Ta sẽ đi từ * và mở rộng về 2 phía cho đến khi gặp một operator khác, lấy exact match. Sau đó lần lượt lấy lần đầu tiên “largest” xuất hiện, từ đó tìm tiếp lần đầu tiên “in the world” xuất hiện (ở sau “largest”).

:coffee: Search within a range of numbers. Put .. between two numbers. For example, camera $50..$100. Đi từ cây trie xuống, lúc đi đảm bảo luôn nằm trong cái range này.

:coffee: Entering “~set” will bring back results that include words like “configure”, “collection” and “change” which are all synonyms of “set”. Tìm một dictionary, sau đó thực hiện set OR configure OR collection OR…

# Cần viết các function chính sau:

**Mỗi operator sẽ cho ra một unordered_map có key là id file và value là số lần xuất hiện**

    - unordered_map <int, int> merge (unordered_map <int, int> &A, unordered_map <int, int> &B)
// Nhận vào A, B, đưa ra hợp của A và B, phép OR

    - unordered_map <int, int> intersect (unordered_map <int, int> &A, unordered_map <int, int> &B)
// Nhận vào A, B, đưa ra giao của A và B, phép AND

    - unordered_map <int, int> exact (unordered_map <int, int> &A, string &key)
// Nhận vào A, đưa ra những đoạn chứa đúng key

    - unordered_map <int, int> fileNameQuery (unordered_map <int, int> &A, string &fileNameRequire)
// Ở fileNameRequire, xử lý cho cả filetype và intitle

    - unordered_map <int, int> wildcard (unordered_map <int, int> &A, vector <string> &orderedKey)
// Nhận vào A, đưa ra những đoạn chứa các string theo thứ tự của orderedKey

    - unordered_map <int, int> search(Trie* root, string &key)
// Đưa ra những đoạn chứa key

    - unordered_map <int, int> range(string &left, string &right)
// Đưa ra những đoạn trong khoảng từ left đến right

# CHIA CÔNG VIỆC
### Dũng:
    - Lên sơ đồ
    - Làm box nhập input và suggestion
    - Chuẩn hóa file
    - Xử lý Trie
    - Làm thống kê
### Hưng:
    - merge, intersect, filenamequery
    - Từ input, xử lý sao cho ra được unordered_map chứa những id file + số lần xuất hiện, và những từ khóa để Hiệp highlight.
### Duy:
    - exact, wildcard, range.
    - Viết report
### Hiệp:
    - Tìm dictionary, lưu lại những unordered_map <string, vector<string> > synonym
    - Làm bảng output, cần thể hiện được rõ những thông tin sau: top 5 output: tên file, đoạn text và highlight từ khóa, số lần xuất hiện.

# File
    - trie.h -> chứa class trie
    - operator.h -> chứa các operator
    - normalize.h -> chuẩn hóa
    - read.h -> đọc dữ liệu vào
    - screen.h -> bảng chọn input và output


