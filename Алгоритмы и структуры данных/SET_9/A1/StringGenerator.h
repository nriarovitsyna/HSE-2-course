#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <vector>
#include <string>
#include <random>

class StringGenerator {
private:
    static const std::string chars;
    static std::mt19937 rng;
    static std::string randomString(int minLen, int maxLen);
    static std::vector<std::string> generateSorted(int count, int minLen, int maxLen);

public:
    static std::vector<std::string> generateRandom(int count, int minLen, int maxLen);
    static std::vector<std::string> generateReversed(int count, int minLen, int maxLen);
    static std::vector<std::string> generateAlmostSorted(int count, int minLen, int maxLen, int swaps = 10);
};

#endif