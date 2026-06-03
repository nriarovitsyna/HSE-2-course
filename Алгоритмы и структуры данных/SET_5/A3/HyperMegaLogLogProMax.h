#ifndef HYPERMEGALOGLOGPROMAX_H
#define HYPERMEGALOGLOGPROMAX_H

#include <string>
#include <bitset>
#include <array>
#include <vector>

class HyperMegaLogLogProMax {
private:
    static const int B = 12;
    static const int m = 1 << B;
    static const int L = 32;
    static const int regBits = 5;
    
    std::bitset<m * regBits> rank;
    double alpha;

    std::array<double, 4> biasTable;
    
    static constexpr uint32_t regVal = (1U << regBits) - 1;
    
    inline uint8_t getRank(int index) const;
    inline void setRank(int index, uint8_t value);

    inline int leadingZeroCount(uint32_t value) const;

public:
    HyperMegaLogLogProMax();
    void add(const std::string& item);
    double estimate() const;
    
    size_t getMemoryUsage() const { return (m * regBits + 7) / 8; }
    void printStats() const;
};

size_t exactUnique(const std::vector<std::string>& part);

#endif