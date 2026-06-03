#ifndef HASHFUNCGEN_H
#define HASHFUNCGEN_H

#include <cstdint>
#include <string>

class HashFuncGen {
public:
    static uint32_t hash(const std::string& key);
    static bool testUniformity(int numTests = 10000, int numBuckets = 1024);
};

#endif