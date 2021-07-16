#ifndef TRIE_H
#define TRIE_H

#include <cctype>
#include <cassert>
#include <unordered_map>
using namespace std;

int _(char c) {
    if (c == '-') return 0;
    if (c == '$') return 1;
    if (c == '#') return 2;
    if (isdigit(c)) return c - '0' + 3;
    if (isalpha(c)) return c - 'a' + 13;
    assert(false);
}

#define MAXCHAR 39
class TRIE {
private:
    struct TrieNode {
        TrieNode *child[MAXCHAR];
        unordered_map <int, int> file;
        TrieNode() {
            for (int i = 0; i < MAXCHAR; i++)
                child[i] = NULL;
        }
    };

    TrieNode* root;

    inline void _insert(TrieNode *cur, const string &s, int idFile) {
        for (char c : s) {
            int _c = _(c);
            if (!cur -> child[_c]) 
                cur -> child[_c] = new TrieNode();
            cur = cur -> child[_c];
        }
        cur -> file[idFile]++;
    }

    inline unordered_map <int, int> _search(TrieNode* cur, const string& s) {
        unordered_map <int, int> ZERO;
        for (char c : s) {
            int _c = _(c);
            if (!cur -> child[_c]) return ZERO;
            cur = cur -> child[_c];
        }
        return cur -> file;
    }

    void destroy(TrieNode *cur) {
        if (cur) {
            for (int i = 0; i < MAXCHAR; i++)
                destroy(cur -> child[i]);
            delete cur;
        }
    }

public:
    TRIE() { root = new TrieNode(); }

    inline void insert(const string& s, int idFile) {
        _insert(root, s, idFile);
    }

    inline unordered_map <int, int> search(const string& s) {
        return _search(root, s);
    }

    ~ TRIE() { destroy(root); }
};

#endif