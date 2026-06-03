#include "SortingAlgorithms.h"
#include <algorithm>

struct StringWithLCP {
    std::string str;
    int lcp;
};

static std::pair<char, int> lcpCompare(const std::string& a, const std::string& b, int k) {
    int n = std::min(a.length(), b.length());
    int i = k;
    while (i < n && a[i] == b[i]) {
        ++stringMergeSortComparisons;
        i++;
    }
    if (i < n) {
        ++stringMergeSortComparisons;
    }
    char cmp;
    if (i == n) {
        if (a.length() == b.length()) cmp = '=';
        else if (a.length() < b.length()) cmp = '<';
        else cmp = '>';
    } else {
        if (a[i] < b[i]) cmp = '<';
        else cmp = '>';
    }
    return std::make_pair(cmp, i);
}

static std::vector<StringWithLCP> merge(const std::vector<StringWithLCP>& P, const std::vector<StringWithLCP>& Q) {
    std::vector<StringWithLCP> result;
    size_t i = 0, j = 0;
    
    while (i < P.size() && j < Q.size()) {
        int ki = P[i].lcp;
        int pj = Q[j].lcp;
        
        if (ki > pj) {
            result.push_back(P[i]);
            i++;
        } 
        else if (ki < pj) {
            result.push_back(Q[j]);
            j++;
        } 
        else {
            std::pair<char, int> cmpRes = lcpCompare(P[i].str, Q[j].str, ki);
            char cmp = cmpRes.first;
            if (cmp == '<') {
                result.push_back(P[i]);
                i++;
            } else {
                result.push_back(Q[j]);
                j++;
            }
        }
    }
    
    while (i < P.size()) {
        result.push_back(P[i]);
        i++;
    }
    while (j < Q.size()) {
        result.push_back(Q[j]);
        j++;
    }
    
    return result;
}

static std::vector<StringWithLCP> stringMergeSortInternal(const std::vector<std::string>& arr, int left, int right) {
    if (left == right) {
        std::vector<StringWithLCP> single;
        single.push_back({arr[left], 0});
        return single;
    }
    
    int mid = (left + right) / 2;
    std::vector<StringWithLCP> P = stringMergeSortInternal(arr, left, mid);
    std::vector<StringWithLCP> Q = stringMergeSortInternal(arr, mid + 1, right);
    
    return merge(P, Q);
}

void stringMergeSort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    stringMergeSortComparisons = 0;
    std::vector<StringWithLCP> sorted = stringMergeSortInternal(arr, 0, arr.size() - 1);
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = sorted[i].str;
    }
}