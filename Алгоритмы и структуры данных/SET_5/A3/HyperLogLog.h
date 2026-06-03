#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <string>
#include <vector>

class HyperLogLog {
private:
    static const int B = 12;
    static const int m = 1 << B;
    static const int L = 32;
    std::vector<uint8_t> rank;
    double alpha;

public:
    HyperLogLog();
    void add(const std::string& item);
    double estimate() const;
};

size_t exactUnique(const std::vector<std::string>& part);

#endif