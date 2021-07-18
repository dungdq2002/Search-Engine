#ifndef OPERATOR_H
#define OPERATOR_H

#include "library.h"

unordered_map<int, int> merge(unordered_map<int, int> &A, unordered_map<int, int> &B)
{
    unordered_map<int, int> result = A;
    for (pair<int, int> b : B)
        result[b.first] += b.second;
    return result;
}

unordered_map<int, int> intersect(unordered_map<int, int> &A, unordered_map<int, int> &B)
{
    unordered_map<int, int> result;
    for (pair<int, int> a : A)
    {
        unordered_map<int, int>::iterator find = B.find(a.first);
        if (find != B.end())
            result[a.first] = a.second + (*find).second;
    }
    return result;
}

unordered_map<int, int> fileNameQuery(unordered_map<int, int> &A, string &fileNameRequire)
{
}

#endif