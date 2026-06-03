#include "StringGenerator.h"
#include <algorithm>
#include <chrono>

const std::string StringGenerator::chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-";

std::mt19937 StringGenerator::rng(std::chrono::steady_clock::now().time_since_epoch().count());

std::string StringGenerator::randomString(int minLen, int maxLen) {
    std::uniform_int_distribution<int> lenDist(minLen, maxLen);
    int length = lenDist(rng);
    std::uniform_int_distribution<int> charDist(0, (int)chars.size() - 1);
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += chars[charDist(rng)];
    }
    return result;
}

std::vector<std::string> StringGenerator::generateSorted(int count, int minLen, int maxLen) {
    std::vector<std::string> arr(count);
    for (int i = 0; i < count; ++i) {
        arr[i] = randomString(minLen, maxLen);
    }
    std::sort(arr.begin(), arr.end());
    return arr;
}

std::vector<std::string> StringGenerator::generateRandom(int count, int minLen, int maxLen) {
    std::vector<std::string> arr(count);
    for (int i = 0; i < count; ++i) {
        arr[i] = randomString(minLen, maxLen);
    }
    return arr;
}

std::vector<std::string> StringGenerator::generateReversed(int count, int minLen, int maxLen) {
    std::vector<std::string> arr = generateSorted(count, minLen, maxLen);
    std::reverse(arr.begin(), arr.end());
    return arr;
}

std::vector<std::string> StringGenerator::generateAlmostSorted(int count, int minLen, int maxLen, int swaps) {
    std::vector<std::string> arr = generateSorted(count, minLen, maxLen);
    std::uniform_int_distribution<int> dist(0, count - 1);
    for (int i = 0; i < swaps; ++i) {
        std::swap(arr[dist(rng)], arr[dist(rng)]);
    }
    return arr;
}