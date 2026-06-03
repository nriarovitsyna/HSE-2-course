#include "bloom_filter.h"

BloomFilter::BloomFilter(size_t hashes, size_t bits) {
    _hashes = hashes;
    _bits = bits;
    _filter.resize(bits, false);
}

void BloomFilter::add(const std::string& item) {
    _raw_set.insert(item);
    for (size_t i = 0; i < _hashes; ++i) {
        size_t hash_value = _get_hash(static_cast<int>(i), item) % _bits;
        _filter[hash_value] = true;
    }
}

bool BloomFilter::verify(const std::string& item) {
    bool present = true;
    for (size_t i = 0; i < _hashes; ++i) {
        size_t hash_value = _get_hash(static_cast<int>(i), item) % _bits;
        if (!_filter[hash_value]) {
            present = false;
            break;
        }
    }
    cnt++;
    if (present && _raw_set.find(item) == _raw_set.end()) {
        false_positive_cnt++;
    }
    return present;
}

double BloomFilter::getFPRate() const {
    if (cnt == 0) return 0.0;
    return static_cast<double>(false_positive_cnt) / cnt;
}

size_t BloomFilter::numberOfHashFunctions() const {
    return _hashes;
}

size_t BloomFilter::numberOfBits() const {
    return _bits;
}

inline size_t BloomFilter::_get_hash(int index, const std::string& item) const {
    if (index == 0) {
        return hasher(item);
    }
    return hasher(item + std::to_string(index));
}