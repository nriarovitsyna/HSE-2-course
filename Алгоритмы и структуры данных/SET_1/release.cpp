#include <stdexcept>
#include "list.h"

List::List() {
    head = nullptr;
    tail = nullptr;
    _size = 0;
}

List::List(const List& other) {
    head = nullptr;
    tail = nullptr;
    _size = 0;

    if (this != &other) {
        copy(other);
    }
}

List::List(std::vector<int> array) {
    head = nullptr;
    tail = nullptr;
    _size = 0;

    for (int value : array) {
        push_back(value);
    }
}

List::~List() {
    clear();
}

int List::front() {
    if (empty()) {
        throw std::runtime_error("Attempt to access front of an empty list");
    }

    return head->value;
}

int List::back() {
    if (empty()) {
        throw std::runtime_error("Attempt to access back of an empty list");
    }
    
    return tail->value;
}

void List::push_back(int value) {
    Node* new_node = new Node(value);
    
    if (empty()) {
        head = tail = new_node;
    } else {
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }

    _size++;
}

void List::push_front(int value) {
    Node* new_node = new Node(value);
    
    if (empty()) {
        head = tail = new_node;
    } else {
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
    }

    _size++;
}

void List::insert(Node* pos, int value) {
    if (pos == nullptr) {
        throw std::runtime_error("Incorrect position!");
    }
    
    bool flag = false;
    Node* iter = head;

    while (iter != nullptr) {
        if (iter == pos) {
            flag = true;
            break;
        }

        iter = iter->next;
    }
    
    if (!flag) {
        throw std::runtime_error("Incorrect position!");
    }
    
    if (pos == tail) {
        push_back(value);
    } else {
        Node* new_node = new Node(value);
        new_node->next = pos->next;
        new_node->prev = pos;

        if (pos->next != nullptr) {
            pos->next->prev = new_node;
        }
        
        pos->next = new_node;
        _size++;
    }
}

void List::pop_front() {
    if (empty()) {
        throw std::runtime_error("Deleting in empty list");
    }
    
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } else {
        Node* old_head = head;
        head = head->next;
        head->prev = nullptr;
        delete old_head;
    }

    _size--;
}

void List::pop_back() {
    if (empty()) {
        throw std::runtime_error("Deleting in empty list");
    }
    
    if (head == tail) {
        delete tail;
        head = tail = nullptr;
    } else {
        Node* old_tail = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete old_tail;
    }

    _size--;
}

void List::erase(Node* pos) {
    if (pos == nullptr) {
        throw std::runtime_error("Incorrect position!");
    }
    
    if (pos == head) {
        pop_front();
    } else if (pos == tail) {
        pop_back();
    } else {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        delete pos;
        _size--;
    }
}

void List::clear() {
    while (!empty()) {
        pop_front();
    }
}

void List::reverse() {
    if (empty() || head == tail) {
        return;
    }
    
    Node* temp = nullptr;
    Node* iter = head;
    
    while (iter != nullptr) {
        temp = iter->prev;
        iter->prev = iter->next;
        iter->next = temp;
        iter = iter->prev;
    }
    
    temp = head;
    head = tail;
    tail = temp;
}

void List::remove_duplicates() {
    if (empty() || head == tail) {
        return;
    }
    
    Node* iter = head;
    
    while (iter != nullptr) {
        Node* search = iter->next;
        Node* last_search = iter;
        
        while (search != nullptr) {
            if (search->value == iter->value) {
                last_search->next = search->next;
                if (search->next != nullptr) {
                    search->next->prev = last_search;
                } else {
                    tail = last_search;
                }
                delete search;
                _size--;
                search = last_search->next;
            } else {
                last_search = search;
                search = search->next;
            }
        }

        iter = iter->next;
    }
}

void List::replace(int old_value, int new_value) {
    Node* iter = head;

    while (iter != nullptr) {
        if (iter->value == old_value) {
            iter->value = new_value;
        }
        iter = iter->next;
    }
}

void List::merge(const List& other) {
    Node* iter = other.head;

    while (iter != nullptr) {
        push_back(iter->value);
        iter = iter->next;
    }
}

bool List::check_cycle() const {
    if (empty()) {
        return false;
    }
    
    Node* tortoise = head;
    Node* hare = head;
    
    while (hare != nullptr && hare->next != nullptr) {
        tortoise = tortoise->next;
        hare = hare->next->next;
        
        if (tortoise == hare) {
            return true;
        }
    }
    
    return false;
}

size_t List::size() const {
    return _size;
}

bool List::empty() const {
    return _size == 0;
}

void List::copy(const List& other) {
    clear();
    
    Node* iter = other.head;

    while (iter != nullptr) {
        push_back(iter->value);
        iter = iter->next;
    }
}