#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <cctype>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;

int _(char c)
{
    if (c == '-')
        return 0;
    if (c == '$')
        return 1;
    if (c == '#')
        return 2;
    if (isdigit(c))
        return c - '0' + 3;
    if (isalpha(c))
        return c - 'a' + 13;
    assert(false);
}

#define _MAXCHAR 39
class TRIE
{
private:
    struct TrieNode
    {
        TrieNode *child[_MAXCHAR];
        unordered_map<int, int> file;
        TrieNode()
        {
            for (int i = 0; i < _MAXCHAR; i++)
                child[i] = NULL;
        }
    };

    TrieNode *root;

    inline void _insert(TrieNode *cur, const string &s, int idFile)
    {
        for (char c : s)
        {
            int _c = _(c);
            if (!cur->child[_c])
                cur->child[_c] = new TrieNode();
            cur = cur->child[_c];
        }
        cur->file[idFile]++;
    }

    inline unordered_map<int, int> _search(TrieNode *cur, const string &s)
    {
        unordered_map<int, int> ZERO;
        for (char c : s)
        {
            int _c = _(c);
            if (!cur->child[_c])
                return ZERO;
            cur = cur->child[_c];
        }
        return cur->file;
    }

    void destroy(TrieNode *cur)
    {
        if (cur)
        {
            for (int i = 0; i < _MAXCHAR; i++)
                destroy(cur->child[i]);
            delete cur;
        }
    }

public:
    TRIE() { root = new TrieNode(); }

    inline void insert(const string &s, int idFile)
    {
        _insert(root, s, idFile);
    }

    inline unordered_map<int, int> search(const string &s)
    {
        return _search(root, s);
    }

    ~TRIE() { destroy(root); }
};


#define _MAXCHAR1 256
class TRIE_SEARCHING_HISTORY
{
private:
    struct TrieNode
    {
        TrieNode *child[_MAXCHAR1];
        int cntEnd;
        TrieNode()
        {
            cntEnd = 0;
            for (int i = 0; i < _MAXCHAR1; i++)
                child[i] = NULL;
        }
    };

    TrieNode *root;

    inline void _insert(TrieNode *cur, const string &s) {
        for (char c : s)
        {
            if (!cur->child[c])
                cur->child[c] = new TrieNode();
            cur = cur->child[c];
        }
        cur -> cntEnd++;
    }

    void _dfs(TrieNode *cur, string& tmp, vector < pair <int, string> > &res) {
        if (cur -> cntEnd) {
            res.push_back({cur -> cntEnd, tmp});
        }
        for (int i = 0; i < 256; i++) {
            if (cur -> child[i]) {
                tmp += char(i);
                _dfs(cur -> child[i], tmp, res);
                tmp.pop_back();
            }
        }
    }

    inline vector <string> _search(TrieNode *cur, const string &s)
    {
        vector <string> ZERO;
        for (char c : s)
        {
            if (!cur->child[c])
                return ZERO;
            cur = cur->child[c];
        }
        string tmp = s;
        vector <pair <int, string> > _get;
        _dfs(cur, tmp, _get);
        sort(_get.rbegin(), _get.rend());
        vector <string> res;
        for (int i = 0; i < min(int(_get.size()), 5); i++) {
            res.push_back(_get[i].second);
        }
        return res;
    }

    void destroy(TrieNode *cur)
    {
        if (cur)
        {
            for (int i = 0; i < _MAXCHAR1; i++)
                destroy(cur->child[i]);
            delete cur;
        }
    }

public:
    TRIE_SEARCHING_HISTORY() { root = new TrieNode(); }

    inline void insert(const string &s)
    {
        _insert(root, s);
    }

    inline vector <string> search(const string &s)
    {
        return _search(root, s);
    }

    ~TRIE_SEARCHING_HISTORY() { destroy(root); }
};


#endif