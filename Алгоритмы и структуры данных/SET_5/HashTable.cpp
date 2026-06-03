#include <vector>
#include <functional>
#include <stdexcept>
#include <cstdint>
#include <cstddef>

template <class KeyType, class ValueType>
struct Node {
    KeyType key;
    ValueType value;
    Node *next;

    Node(KeyType k, ValueType v) : key(k), value(v), next(nullptr) {}
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
private:
    std::vector<Node<KeyType, ValueType>*> buckets;
    size_t capacity_;
    size_t size_;
    double maxLoadFactor;
    Func hashFunc;

    void rehash() {
        size_t newCapacity = capacity_ * 2;
        if (newCapacity == 0) newCapacity = 1;
        std::vector<Node<KeyType, ValueType>*> newBuckets(newCapacity, nullptr);

        for (auto head : buckets) {
            Node<KeyType, ValueType>* current = head;
            while (current) {
                Node<KeyType, ValueType>* nextNode = current->next;
                size_t newHash = hashFunc(current->key) % newCapacity;
                current->next = newBuckets[newHash];
                newBuckets[newHash] = current;
                current = nextNode;
            }
        }

        buckets = std::move(newBuckets);
        capacity_ = newCapacity;
    }

public:
    HashTable() : HashTable(100, 0.5) {}

    HashTable(Func h) : HashTable(100, 0.5, h) {}

    HashTable(size_t cap, double lf, Func h = std::hash<KeyType>())
        : capacity_(cap == 0 ? 100 : cap),
          size_(0),
          maxLoadFactor((lf > 0.0 && lf <= 1.0) ? lf : 0.5),
          hashFunc(std::move(h)),
          buckets(capacity_, nullptr) {}

    ~HashTable() {
        for (auto head : buckets) {
            Node<KeyType, ValueType>* current = head;
            while (current) {
                Node<KeyType, ValueType>* nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }
    }

    int size() const {
        return static_cast<int>(size_);
    }

    int capacity() const {
        return static_cast<int>(capacity_);
    }

    void insert(KeyType key, ValueType value) {
        if (capacity_ == 0) return;
        size_t hashVal = hashFunc(key) % capacity_;

        Node<KeyType, ValueType>* current = buckets[hashVal];
        Node<KeyType, ValueType>* prev = nullptr;
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            prev = current;
            current = current->next;
        }

        Node<KeyType, ValueType>* newNode = new Node<KeyType, ValueType>(key, value);
        if (prev) {
            prev->next = newNode;
        } else {
            buckets[hashVal] = newNode;
        }
        size_++;

        if (static_cast<double>(size_) / capacity_ > maxLoadFactor) {
            rehash();
        }
    }

    ValueType* find(KeyType key) {
        if (capacity_ == 0) return nullptr;
        size_t hashVal = hashFunc(key) % capacity_;
        Node<KeyType, ValueType>* current = buckets[hashVal];
        while (current) {
            if (current->key == key) {
                return &current->value;
            }
            current = current->next;
        }
        return nullptr;
    }

    void erase(KeyType key) {
        if (capacity_ == 0) return;
        size_t hashVal = hashFunc(key) % capacity_;
        Node<KeyType, ValueType>* current = buckets[hashVal];
        Node<KeyType, ValueType>* prev = nullptr;
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    buckets[hashVal] = current->next;
                }
                delete current;
                size_--;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    Node<KeyType, ValueType>& operator[](uint64_t idx) {
        if (idx >= static_cast<uint64_t>(capacity_)) {
            throw std::out_of_range("Hash code out of range");
        }
        if (buckets[idx] == nullptr) {
            throw std::runtime_error("Empty bucket");
        }
        return *buckets[idx];
    }

    Node<KeyType, ValueType> at(uint64_t idx) {
        if (idx >= static_cast<uint64_t>(capacity_)) {
            throw std::out_of_range("Hash code out of range");
        }
        if (buckets[idx] == nullptr) {
            throw std::runtime_error("Empty bucket");
        }
        return *buckets[idx];
    }
};