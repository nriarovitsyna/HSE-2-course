#include "HashFuncGen.h"
#include "RandomStreamGen.h"
#include <vector>
#include <cmath>

uint32_t HashFuncGen::hash(const std::string& key) {
    uint32_t hashVal = 2166136261U;
    for (char c : key) {
        hashVal ^= static_cast<uint8_t>(c);
        hashVal *= 16777619U;
    }
    return hashVal;
}

bool HashFuncGen::testUniformity(int numTests, int numBuckets) {
    std::vector<int> buckets(numBuckets, 0);
    RandomStreamGen gen;
    gen.generate(numTests);
    for (const auto& s : gen.getFullStream()) {
        uint32_t hashVal = hash(s);
        buckets[hashVal % numBuckets]++;
    }

    double mean = static_cast<double>(numTests) / numBuckets;
    double variance = 0.0;
    for (int cnt : buckets) {
        variance += (cnt - mean) * (cnt - mean);
    }
    variance /= numBuckets;
    double stdDev = std::sqrt(variance);
    return stdDev < mean * 0.3;
}