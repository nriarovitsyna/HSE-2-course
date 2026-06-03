#include "RandomStreamGen.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

RandomStreamGen::RandomStreamGen(unsigned int seed) 
    : rng(seed), 
      lenDist(1, 30),
      charDist(0, chars.size() - 1) {
}

void RandomStreamGen::generate(int numItems) {
    stream.clear();
    stream.reserve(numItems);
    for (int i = 0; i < numItems; ++i) {
        int len = lenDist(rng);
        std::string s;
        s.reserve(len);
        for (int j = 0; j < len; ++j) {
            s += chars[charDist(rng)];
        }
        stream.push_back(std::move(s));
    }
}

std::vector<std::string> RandomStreamGen::getPart(double percent) const {
    if (stream.empty()) return {};
    size_t count = static_cast<size_t>(stream.size() * percent / 100.0);
    count = std::min(count, stream.size());
    return {stream.begin(), stream.begin() + count};
}

const std::vector<std::string>& RandomStreamGen::getFullStream() const { 
    return stream; 
}

size_t RandomStreamGen::size() const { 
    return stream.size(); 
}

const std::string RandomStreamGen::chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-";