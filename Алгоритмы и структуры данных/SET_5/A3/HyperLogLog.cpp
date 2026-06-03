#include "HyperLogLog.h"
#include "HashFuncGen.h"
#include <unordered_set>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>

HyperLogLog::HyperLogLog() : rank(m, 0) {
    if (m == 16) alpha = 0.673;
    else if (m == 32) alpha = 0.697;
    else if (m == 64) alpha = 0.709;
    else alpha = 0.7213 / (1 + 1.079 / m);
}

void HyperLogLog::add(const std::string& item) {
    uint32_t hashVal = HashFuncGen::hash(item);
    int index = hashVal >> (L - B);
    uint32_t lastBits = hashVal & ((1U << (L - B)) - 1);
    int r;
    if (lastBits == 0) {
        r = (L - B) + 1;
    } else {
        r = 1;
        while ((lastBits & (1U << 31)) == 0) {
            lastBits <<= 1;
            r++;
        }
        r -= B;
    }
    if (r > rank[index]) rank[index] = r;
}

double HyperLogLog::estimate() const {
    double sum = 0.0;
    for (uint8_t r : rank) {
        sum += std::pow(2.0, -r);
    }
    double E = alpha * m * m / sum;

    if (E <= 2.5 * m) {
        int zeros = std::count(rank.begin(), rank.end(), 0);
        if (zeros > 0) E = m * std::log(static_cast<double>(m) / zeros);
    } else if (E > (1LL << 32) / 30.0) {
        E = -(1LL << 32) * std::log(1.0 - E / (1LL << 32));
    }
    return E;
}

size_t exactUnique(const std::vector<std::string>& part) {
    std::unordered_set<std::string> s(part.begin(), part.end());
    return s.size();
}