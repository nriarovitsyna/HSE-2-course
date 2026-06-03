#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <functional>
#include <chrono>
#include <vector>
#include <string>

class StringSortTester {
private:
    static long long comparisonCount;

public:
    static void resetComparisons();
    static long long getComparisonCount();
    static bool compareChars(char a, char b);
    static bool lessThan(const std::string& a, const std::string& b);
    static long long measureTime(std::vector<std::string> arr, 
                                 std::function<void(std::vector<std::string>&)> sortFunc);
};

#endif