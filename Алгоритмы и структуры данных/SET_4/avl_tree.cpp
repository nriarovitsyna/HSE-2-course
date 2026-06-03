#include "avl_tree.h"

Node::Node(int value) : height(1), left(nullptr), right(nullptr), value(value) {}

Node::~Node() {}

AVLTree::AVLTree() : root_(nullptr), size_(0) {}

AVLTree::AVLTree(int value) : root_(new Node(value)), size_(1) {}

int AVLTree::getHeight() {
    return root_ ? root_->height : 0;
}

void AVLTree::insert(int value) {
    root_ = insertNode(root_, value);
}

void AVLTree::erase(int value) {
    root_ = removeNode(root_, value);
}

int *AVLTree::find(int value) {
    Node* node = findNode(root_, value);
    return node ? &node->value : nullptr;
}

int *AVLTree::traversal() {
    if (size_ == 0) return nullptr;
    
    int* result = new int[size_];
    int index = 0;
    traversalInternal(root_, result, &index);
    return result;
}

int *AVLTree::lowerBound(int value) {
    Node* node = lowerBoundInternal(root_, value);
    return node ? &node->value : nullptr;
}

bool AVLTree::empty() {
    return size_ == 0;
}

Node *AVLTree::getRoot() {
    return root_;
}

int AVLTree::getSize() {
    return size_;
}

AVLTree::~AVLTree() {
    auto deleteTree = [](auto& self, Node* node) -> void {
        if (!node) return;
        self(self, node->left);
        self(self, node->right);
        delete node;
    };
    
    deleteTree(deleteTree, root_);
    root_ = nullptr;
    size_ = 0;
}

int AVLTree::getNodeHeight(Node *node) {
    return node ? node->height : 0;
}

int AVLTree::balanceFactor(Node *node) {
    if (!node) return 0;
    return getNodeHeight(node->left) - getNodeHeight(node->right);
}

void AVLTree::balanceHeight(Node *node) {
    if (!node) return;
    int left_height = getNodeHeight(node->left);
    int right_height = getNodeHeight(node->right);
    node->height = (left_height > right_height ? left_height : right_height) + 1;
}

Node *AVLTree::rotateRight(Node *node) {
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    
    balanceHeight(node);
    balanceHeight(new_root);
    
    return new_root;
}

Node *AVLTree::rotateLeft(Node *node) {
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    
    balanceHeight(node);
    balanceHeight(new_root);
    
    return new_root;
}

Node *AVLTree::balanceNode(Node *node) {
    if (!node) return nullptr;
    
    balanceHeight(node);
    
    int balance = balanceFactor(node);
    
    if (balance > 1 && balanceFactor(node->left) >= 0) {
        return rotateRight(node);
    }
    
    if (balance < -1 && balanceFactor(node->right) <= 0) {
        return rotateLeft(node);
    }
    
    if (balance > 1 && balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if (balance < -1 && balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

Node *AVLTree::insertNode(Node *node, int value) {
    if (!node) {
        size_++;
        return new Node(value);
    }
    
    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value);
    } else {
        return node;
    }
    
    return balanceNode(node);
}

Node *AVLTree::findMinNode(Node *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

Node *AVLTree::removeMinNode(Node *node) {
    if (!node->left) {
        return node->right;
    }
    
    node->left = removeMinNode(node->left);
    return balanceNode(node);
}

Node *AVLTree::removeNode(Node *node, int value) {
    if (!node) return nullptr;
    
    if (value < node->value) {
        node->left = removeNode(node->left, value);
    } else if (value > node->value) {
        node->right = removeNode(node->right, value);
    } else {
        Node* left = node->left;
        Node* right = node->right;
        
        delete node;
        size_--;
        
        if (!right) return left;
        
        Node* min_node = findMinNode(right);
        min_node->right = removeMinNode(right);
        min_node->left = left;
        
        return balanceNode(min_node);
    }
    
    return balanceNode(node);
}

Node *AVLTree::findNode(Node *node, int value) {
    while (node) {
        if (value < node->value) {
            node = node->left;
        } else if (value > node->value) {
            node = node->right;
        } else {
            return node;
        }
    }
    return nullptr;
}

void AVLTree::traversalInternal(Node *node, int *array, int *index) {
    if (!node) return;
    
    traversalInternal(node->left, array, index);
    array[(*index)++] = node->value;
    traversalInternal(node->right, array, index);
}

Node *AVLTree::lowerBoundInternal(Node *current, int value) const {
    Node* result = nullptr;
    
    while (current) {
        if (current->value >= value) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return result;
}