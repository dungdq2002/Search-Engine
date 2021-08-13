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

typedef unordered_map<int, unordered_map<string, int>> RESULT_MAP;
typedef pair<int, unordered_map<string, int>> RESULT_PAIR;

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
    return 39;
    assert(false);
}

#define __MAXCHAR 40
class TRIE
{
private:
    struct TrieNode
    {
        TrieNode *child[__MAXCHAR];
        unordered_map<int, int> file;
        TrieNode()
        {
            for (int i = 0; i < __MAXCHAR; i++)
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

    void add(unordered_map<int, int> &A, unordered_map<int, int> &B) {
        for (auto it: B) A[it.first] += it.second;
    }

    string ___tmp___;
    void _inRange(TrieNode *cur, const string &left, const string &right, int id, bool okLeft, bool okRight, RESULT_MAP &res) {
        if (id == 10) {
            auto cntFile = cur -> file;
            string yoyo = ___tmp___;
            int p = yoyo[0] == '$' ? 1 : 0;
            while (yoyo[p] == '0') yoyo.erase(yoyo.begin() + p);
            for (auto it : cntFile) {
                res[it.first][yoyo] += it.second;
            }
            return;
        } 
        if (left[id] == '$') {
            assert(right[id] == '$');
            if (!cur -> child[_('$')]) return;
            ___tmp___ += '$';
            _inRange(cur -> child[_('$')], left, right, id+1, okLeft, okRight, res);
            ___tmp___.pop_back();
        }
        for (char c = '0'; c <= '9'; c++) {
            if (!cur -> child[_(c)]) continue;
            bool nOkLeft = okLeft, nOkRight = okRight;
            if (okLeft || c >= left[id]) {
                if (c > left[id]) nOkLeft = true;
            } else continue;
            if (okRight || c <= right[id]) {
                if (c < right[id]) nOkRight = true;
            } else continue;
            ___tmp___ += c;
            _inRange(cur -> child[_(c)], left, right, id+1, nOkLeft, nOkRight, res);
            ___tmp___.pop_back();
        }
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
            for (int i = 0; i < __MAXCHAR; i++)
                destroy(cur->child[i]);
            delete cur;
        }
    }

public:
    TRIE() { root = new TrieNode(); }

    inline void insert(const string &s, int idFile)
    {
        string t = s;
        if (t[0] == '$' || isdigit(t[0])) {
            int p = t[0] == '$' ? 1 : 0;
            while (t.size() < 10) t.insert(p, "0");
        }
        _insert(root, t, idFile);
    }

    inline unordered_map<int, int> search(const string &s)
    {
        string t = s;
        if (t[0] == '$' || isdigit(t[0])) {
            int p = t[0] == '$' ? 1 : 0;
            while (t.size() < 10) t.insert(p, "0");
        }
        return _search(root, t);
    }

    void inRange(const string &left, const string &right, RESULT_MAP &res) {
        string _left = left;
        if (_left[0] == '$' || isdigit(_left[0])) {
            int p = _left[0] == '$' ? 1 : 0;
            while (_left.size() < 10) _left.insert(p, "0");
        }
        string _right = right;
        if (_right[0] == '$' || isdigit(_right[0])) {
            int p = _right[0] == '$' ? 1 : 0;
            while (_right.size() < 10) _right.insert(p, "0");
        }
        cout << " hehe " <<  _left << ' ' << _right << '\n';
        _inRange(root, _left, _right, 0, 0, 0, res);
    }

    ~TRIE() { destroy(root); }
};


#define __MAXCHAR1 256
class TRIE_SEARCHING_HISTORY
{
private:
    struct TrieNode
    {
        TrieNode *child[__MAXCHAR1];
        int cntEnd;
        TrieNode()
        {
            cntEnd = 0;
            for (int i = 0; i < __MAXCHAR1; i++)
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
            for (int i = 0; i < __MAXCHAR1; i++)
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