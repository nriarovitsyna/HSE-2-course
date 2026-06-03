#include "btree.h"
#include <algorithm>
#include <functional>

Node::Node(int t) : parent(nullptr), isLeaf(true), t(t) {}

BTree::BTree(int t): root(nullptr), t_(t), size_(0){}

BTree::~BTree() 
{
    std::function<void(Node*)> delete_node = [&](Node* node) {
        if (!node) return;
        for (Node* child : node->children) {
            delete_node(child);
        }
        delete node;
    };
    delete_node(root);
}

bool keyExists(Node* node, int key) 
{
    if (!node) return false;
    
    for (int k : node->key) {
        if (k == key) return true;
    }
    
    if (node->isLeaf) return false;

    int child_idx = 0;
    while (child_idx < node->key.size() && key > node->key[child_idx]) {
        ++child_idx;
    }
    return keyExists(node->children[child_idx], key);
}

void splitChild(Node* parent, int child_idx, int t, int& tree_size) 
{
    Node* full_child = parent->children[child_idx];
    Node* new_child = new Node(t);
    new_child->isLeaf = full_child->isLeaf;
    
    int med_key = full_child->key[t - 1];
    
    for (int i = 0; i < t - 1; ++i) {
        new_child->key.push_back(full_child->key[i + t]);
    }
    
    full_child->key.resize(t - 1);

    if (!full_child->isLeaf) {
        for (int i = 0; i < t; ++i) {
            new_child->children.push_back(full_child->children[i + t]);
            new_child->children.back()->parent = new_child;
        }
        full_child->children.resize(t);
    }
    
    int insert_idx = 0;
    while (insert_idx < parent->key.size() && med_key > parent->key[insert_idx]) {
        ++insert_idx;
    }
    parent->key.insert(parent->key.begin() + insert_idx, med_key);
    
    parent->children.insert(parent->children.begin() + child_idx + 1, new_child);
    new_child->parent = parent;

    ++tree_size;
}

void insertNonFull(Node* cur_node, int key, int t, int& tree_size) 
{
    if (cur_node->isLeaf) {
        cur_node->key.push_back(key);

        for (int i = cur_node->key.size() - 1; i > 0 && cur_node->key[i] < cur_node->key[i - 1]; --i) {
            std::swap(cur_node->key[i], cur_node->key[i - 1]);
        }
    } else {
        int child_idx = cur_node->key.size() - 1;
        while (child_idx >= 0 && key < cur_node->key[child_idx]) {
            --child_idx;
        }
        ++child_idx;

        if (cur_node->children[child_idx]->key.size() == 2 * t - 1) {
            splitChild(cur_node, child_idx, t, tree_size);
            if (key > cur_node->key[child_idx]) {
                ++child_idx;
            }
        }
        
        insertNonFull(cur_node->children[child_idx], key, t, tree_size);
    }
}

void BTree::insert(int key) 
{
    if (keyExists(root, key)) {
        return;
    }
    
    if (!root) {
        root = new Node(t_);
        root->key.push_back(key);
        size_++;
        return;
    }
    
    if (root->key.size() == 2 * t_ - 1) {
        Node* new_root = new Node(t_);
        new_root->isLeaf = false;
        new_root->children.push_back(root);
        root->parent = new_root;
        
        size_++;
        splitChild(new_root, 0, t_, size_);
        root = new_root;
    }
    
    insertNonFull(root, key, t_, size_);
}

size_t BTree::size() const 
{
    return size_;
}

int64_t BTree::sum() const 
{
    int64_t total = 0;
    
    std::function<void(Node*)> sumLeaves = [&](Node* node) {
        if (!node) return;
        
        if (node->isLeaf) {
            for (int k : node->key) {
                total += k;
            }
        } else {
            for (Node* child : node->children) {
                sumLeaves(child);
            }
        }
    };
    
    sumLeaves(root);
    return total;
}