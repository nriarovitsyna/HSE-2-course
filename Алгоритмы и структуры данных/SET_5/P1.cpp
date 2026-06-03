#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <functional>

template <class KeyType, class ValueType>
struct Node {
    KeyType key;
    ValueType value;
    Node* next;

    Node(KeyType key, ValueType value) : key(key), value(value), next(nullptr) {}
};

template <class KeyType, class ValueType, class HashFunc = std::hash<KeyType>>
class HashTable {
public:
    HashTable() : capacity_(100), loadFactor_(0.5), hashFunc_(), table_(100, nullptr), elementCnt_(0) {}
    
    HashTable(HashFunc hash_func) : capacity_(100), loadFactor_(0.5), hashFunc_(hash_func), table_(100, nullptr), elementCnt_(0) {}
    
    HashTable(size_t capacity, double load_factor, HashFunc hash_func = std::hash<KeyType>()) {
        if (load_factor <= 0.0 || load_factor > 1.0) {
            load_factor = 0.5;
        }
        capacity_ = capacity;
        loadFactor_ = load_factor;
        hashFunc_ = hash_func;
        table_.assign(capacity, nullptr);
        elementCnt_ = 0;
    }
    
    ~HashTable() {
        for (auto bucket_head : table_) {
            Node<KeyType, ValueType>* current = bucket_head;
            while (current) {
                Node<KeyType, ValueType>* next_node = current->next;
                delete current;
                current = next_node;
            }
        }
    }
    
    void insert(KeyType key, ValueType value) {
        if (capacity_ == 0) return;
        size_t hash_value = hashFunc_(key) % capacity_;
        Node<KeyType, ValueType>* current = table_[hash_value];
        Node<KeyType, ValueType>* previous = nullptr;
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            previous = current;
            current = current->next;
        }
        Node<KeyType, ValueType>* new_node = new Node<KeyType, ValueType>(key, value);
        if (previous == nullptr) {
            table_[hash_value] = new_node;
        } else {
            previous->next = new_node;
        }
        elementCnt_++;
        if (static_cast<double>(elementCnt_) / capacity_ > loadFactor_) {
            rehash();
        }
    }
    
    ValueType* find(KeyType key) {
        if (capacity_ == 0) return nullptr;
        size_t hash_value = hashFunc_(key) % capacity_;
        Node<KeyType, ValueType>* current = table_[hash_value];
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
        size_t hash_value = hashFunc_(key) % capacity_;
        Node<KeyType, ValueType>* current = table_[hash_value];
        Node<KeyType, ValueType>* previous = nullptr;
        while (current) {
            if (current->key == key) {
                if (previous == nullptr) {
                    table_[hash_value] = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                elementCnt_--;
                return;
            }
            previous = current;
            current = current->next;
        }
    }
    
    Node<KeyType, ValueType>& operator[](uint64_t index) {
        if (index >= capacity_) {
            throw std::out_of_range("ОШИБКА: Индекс выходит за пределы диапазона.");
        }
        if (table_[index] == nullptr) {
            throw std::runtime_error("ОШИБКА: Ячейка пустая.");
        }
        return *table_[index];
    }
    
    Node<KeyType, ValueType> at(uint64_t index) {
        if (index >= capacity_) {
            throw std::out_of_range("ОШИБКА: Индекс выходит за пределы диапазона.");
        }
        if (table_[index] == nullptr) {
            throw std::runtime_error("ОШИБКА: Ячейка пустая.");
        }
        return *table_[index];
    }
    
    int size() const {
        return static_cast<int>(elementCnt_);
    }
    
    int capacity() const {
        return static_cast<int>(capacity_);
    }
    
private:
    size_t capacity_;
    double loadFactor_;
    HashFunc hashFunc_;
    std::vector<Node<KeyType, ValueType>*> table_;
    size_t elementCnt_;
    
    void rehash() {
        size_t new_capacity = 2 * capacity_;
        if (new_capacity == 0) new_capacity = 1;
        std::vector<Node<KeyType, ValueType>*> new_table(new_capacity, nullptr);
        for (size_t i = 0; i < table_.size(); ++i) {
            Node<KeyType, ValueType>* current = table_[i];
            table_[i] = nullptr;
            while (current) {
                Node<KeyType, ValueType>* next_node = current->next;
                current->next = nullptr;
                size_t new_hash = hashFunc_(current->key) % new_capacity;
                if (new_table[new_hash] == nullptr) {
                    new_table[new_hash] = current;
                } else {
                    Node<KeyType, ValueType>* tail = new_table[new_hash];
                    while (tail->next) {
                        tail = tail->next;
                    }
                    tail->next = current;
                }
                current = next_node;
            }
        }
        table_ = std::move(new_table);
        capacity_ = new_capacity;
    }
};
