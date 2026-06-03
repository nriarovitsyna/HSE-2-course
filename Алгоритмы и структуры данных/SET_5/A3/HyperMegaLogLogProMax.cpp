#include "HyperMegaLogLogProMax.h"
#include "HashFuncGen.h"
#include <unordered_set>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>

HyperMegaLogLogProMax::HyperMegaLogLogProMax() {
    if (m == 16) alpha = 0.673;
    else if (m == 32) alpha = 0.697;
    else if (m == 64) alpha = 0.709;
    else alpha = 0.7213 / (1 + 1.079 / m);
    biasTable = {0.0, 0.1 * m, 0.15 * m, 0.05 * m};
}

uint8_t HyperMegaLogLogProMax::getRank(int index) const {
    int bitIdx = index * regBits;
    uint8_t value = 0;
    for (int i = 0; i < regBits; ++i) {
        value |= rank[bitIdx + i] << i;
    }
    return value;
}

void HyperMegaLogLogProMax::setRank(int index, uint8_t value) {
    int bitIdx = index * regBits;
    for (int i = 0; i < regBits; ++i) {
        rank[bitIdx + i] = (value >> i) & 1;
    }
}

void HyperMegaLogLogProMax::add(const std::string& item) {
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
    if (r > getRank(index)) setRank(index, r);
}

double HyperMegaLogLogProMax::estimate() const {
    double sum = 0.0;
    for (int i = 0; i < m; ++i) {
        sum += std::pow(2.0, -static_cast<double>(getRank(i)));
    }
    double E = alpha * m * m / sum;

    if (E >= m && E < 5 * m) {
        int zone = static_cast<int>((E / m) - 1);
        E -= biasTable[zone];
    }

    if (E <= 2.5 * m) {
        int zeros = 0;
        for (int i = 0; i < m; ++i) if (getRank(i) == 0) ++zeros;
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