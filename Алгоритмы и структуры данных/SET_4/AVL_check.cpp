#include <iostream>
#include <algorithm>
#include <cmath>

struct Node {
    int key;
    Node* left;
    Node* right;
    
    Node(int value) : key(value), left(nullptr), right(nullptr) {}
};

struct BinaryTree {
    Node* root;
    
    BinaryTree() : root(nullptr) {}
    
    void insert(int key) {
        root = insertRec(root, key);
    }
    
    bool isAVL() const {
        return isAVLRec(root);
    }
    
private:
    int height(const Node* node) const {
        if (node == nullptr) {
            return -1;
        }
        return 1 + std::max(height(node->left), height(node->right));
    }

    Node* insertRec(Node* node, int key) {
        if (node == nullptr) {
            return new Node(key);
        }
        if (key < node->key) {
            node->left = insertRec(node->left, key);
        } else if (key > node->key) {
            node->right = insertRec(node->right, key);
        }
        return node;
    }
    
    bool isAVLRec(const Node* node) const {
        if (node == nullptr) {
            return true;
        }
        
        int left_height = height(node->left);
        int right_height = height(node->right);
        int diff = left_height - right_height;
        
        if (std::abs(diff) > 1) {
            return false;
        }
        
        return isAVLRec(node->left) && isAVLRec(node->right);
    }
};

int main() {
    BinaryTree tree;
    int value;
    
    while (std::cin >> value && value != 0) {
        tree.insert(value);
    }
    
    if (tree.isAVL()) {
        std::cout << "YES" << '\n';
    } else {
        std::cout << "NO" << '\n';
    }
    
    return 0;
}