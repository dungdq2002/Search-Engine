# Search Engine

Sample Dataset: [here](https://drive.google.com/file/d/1fpCBh1ZhIrcx0v3htbpNunzTcmntBJuO/view?usp=sharing)

## Requirements

- Operating System: Windows 10 or higher
- C++ 17

## Strategy

- The text paragraphs will be stripped of punctuation marks, converted to lowercase, and all stop words will be removed. The characters '#' and '$' will be preserved.

- The text paragraphs will be added to a Trie data structure, one word at a time. At the last node of each word, there will be an `unordered_map<int, int>` to store the occurrences of that word in each file, with the key being the file ID and the value being the frequency of that word in the file. For numbers, leading zeros will be added to make each number 10 digits long, facilitating range-based operations. For example, given three text paragraphs: "1.txt": "Search engine", "2.txt": "engine vs engine in england", "3.txt": "#abc $99 123456789", [sample Trie](https://studenthcmusedu-my.sharepoint.com/:i:/g/personal/20125127_student_hcmus_edu_vn/EamnA0oCfpFDsLKpZr9cbSMB84PErnz8LXeD_pMI-d4YFA?e=Wf3Dga).

- If it is a regular text paragraph, such as 'Search engine', it will be processed as "Search" OR "Engine". The priority will be based on the total number of occurrences of both words.
- The operators will be processed from left to right.

# Operators:

- `AND` Operator: This type of query will be processed as follows: <Left-hand side search result> `AND` <Right-hand side search result>, meaning both the left and right sides need to be found within the text. The process involves finding the matching files on the left side and the matching files on the right side, then intersecting these sets and finding the subset. The priority is based on the total number of occurrences of both the left and right sides.

- `OR` Operator: Similar to regular search, it retrieves results containing either the left or the right side.

- `-` Operator: This type of query is used to find text that does not contain the word following the `-` symbol. To execute this, we first find all files containing the remaining words, and then exclude the files that have the word specified after the `-` symbol.

- `intitle:hammer nails`: For this type of query, after finding all the text containing the specified word, we filter out the text with the title "hammer". Note that here we only process the "hammer" in the title and do not consider the entire phrase "hammer nails" in the title.

- `+` Operator: Functions similar to the `AND` operator.

- `filetype:txt`: Similar to `intitle`, but filters based on file types.

- Searching for a price: Prefix the number with `"$"`, for example: camera $400. It is searched as usual.

- Searching for hashtags: Prefix the word with `"#"`, for example: #throwbackthursday. It is searched as usual.

- Searching for an exact match: Enclose the word or phrase in `" "`, for example: "tallest building". Filter the text to include both "tallest" and "building", then count the occurrences of the phrase "tallest building".

- Searching for wildcards or unknown words: Use `"*"` as a placeholder in the word or phrase where you want to leave a placeholder, for example: "largest _ in the world". Filter the text to include "largest" `AND` "in the world". Note that the method of extracting the search word is different here. We start from `"_"` and expand in both directions until we encounter another operator, and then take an exact match. Next, we find the first occurrence of "largest" and continue to find the first occurrence of "in the world" (after "largest").

- Searching within a range of numbers: Use `".."` between two numbers, for example: camera $50..$100. Traverse the Trie data structure, ensuring that the search stays within this range.

- Entering `~set` will bring back results that include words like "configure," "collection," and "change," which are all synonyms of "set". Find a dictionary, and then perform the search with "set" `OR` "configure" `OR` "collection" `OR`...

Please note that the explanations provided here outline the general approach for processing these operators in a search system. Implementation details and specific optimizations may vary.

## Running

```
cd Engine
g++ -std=c++17 main.cpp -o main && main.exe
```

## Demo

- Searching screen

![](https://i.imgur.com/D0AdKqe.png)

- Searching result

![](https://i.imgur.com/6J0RvXf.png)

- Reading file

![](https://i.imgur.com/UhX4ifb.png)
