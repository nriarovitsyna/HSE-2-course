#include "StringSortTester.h"

long long StringSortTester::comparisonCount = 0;

void StringSortTester::resetComparisons() {
    comparisonCount = 0;
}

long long StringSortTester::getComparisonCount() {
    return comparisonCount;
}

bool StringSortTester::compareChars(char a, char b) {
    ++comparisonCount;
    return a < b;
}

bool StringSortTester::lessThan(const std::string& a, const std::string& b) {
    int n = std::min(a.length(), b.length());
    for (int i = 0; i < n; i++) {
        if (compareChars(a[i], b[i])) return true;
        if (compareChars(b[i], a[i])) return false;
    }
    return a.length() < b.length();
}

long long StringSortTester::measureTime(std::vector<std::string> arr, 
                                         std::function<void(std::vector<std::string>&)> sortFunc) {
    resetComparisons();
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}