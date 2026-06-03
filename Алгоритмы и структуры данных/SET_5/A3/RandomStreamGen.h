#ifndef RANDOMSTREAMGEN_H
#define RANDOMSTREAMGEN_H

#include <vector>
#include <string>
#include <random>

class RandomStreamGen {
private:
    std::vector<std::string> stream;
    static const std::string chars;
    std::mt19937 rng;
    std::uniform_int_distribution<int> lenDist;
    std::uniform_int_distribution<int> charDist;

public:
    RandomStreamGen(unsigned int seed = std::random_device{}());
    void generate(int numItems);
    std::vector<std::string> getPart(double percent) const;
    const std::vector<std::string>& getFullStream() const;
    size_t size() const;
};

#endif