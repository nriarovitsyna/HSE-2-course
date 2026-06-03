#include "splay-tree.h"
#include <algorithm>

Node::Node(int key, Node* parent) : key(key), left(nullptr), right(nullptr), parent(parent) {}

SplayTree::SplayTree() : root(nullptr) {}

SplayTree::~SplayTree() {
    clear(root);
}

void SplayTree::clear(Node* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void SplayTree::rotateLeft(Node* node) {
    Node* right_child = node->right;
    if (!right_child) return;

    node->right = right_child->left;
    if (right_child->left) {
        right_child->left->parent = node;
    }

    right_child->parent = node->parent;
    if (!node->parent) {
        root = right_child;
    } else if (node == node->parent->left) {
        node->parent->left = right_child;
    } else {
        node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
}

void SplayTree::rotateRight(Node* node) {
    Node* left_child = node->left;
    if (!left_child) return;

    node->left = left_child->right;
    if (left_child->right) {
        left_child->right->parent = node;
    }

    left_child->parent = node->parent;
    if (!node->parent) {
        root = left_child;
    } else if (node == node->parent->left) {
        node->parent->left = left_child;
    } else {
        node->parent->right = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
}

void SplayTree::insert(int key) {
    if (!root) {
        root = new Node(key, nullptr);
        return;
    }

    Node* node = root;
    Node* parent = nullptr;

    while (node) {
        if (key == node->key) {
            return;
        }
        parent = node;
        node = (key < node->key) ? node->left : node->right;
    }

    Node* new_node = new Node(key, parent);
    if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

Node* SplayTree::find(int key) const {
    Node* node = root;
    while (node) {
        if (key == node->key) {
            return node;
        }
        node = (key < node->key) ? node->left : node->right;
    }
    return nullptr;
}

int SplayTree::splay(Node* node) {
    if (!node) return 0;

    int cnt_rotat = 0;

    while (node->parent) {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;

        if (!grandparent) {
            if (node == parent->left) {
                rotateRight(parent);
            } else {
                rotateLeft(parent);
            }
            cnt_rotat++;
        } else if (node == parent->left && parent == grandparent->left) {
            rotateRight(grandparent);
            rotateRight(parent);
            cnt_rotat += 2;
        } else if (node == parent->right && parent == grandparent->right) {
            rotateLeft(grandparent);
            rotateLeft(parent);
            cnt_rotat += 2;
        } else {
            if (node == parent->left && parent == grandparent->right) {
                rotateRight(parent);
                rotateLeft(grandparent);
            } else {
                rotateLeft(parent);
                rotateRight(grandparent);
            }
            cnt_rotat++;
        }
    }

    root = node;
    return cnt_rotat;
}

int SplayTree::getHeight() const {
    return getHeight(root);
}

int SplayTree::getHeight(Node* node) const {
    if (!node) return -1;
    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}