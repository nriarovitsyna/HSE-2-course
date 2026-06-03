#include "rbtree.h"
#include <algorithm>
#include <cassert>
#include <stack>

Node::Node(int key): key(key), height(0), size(1), left(nullptr), right(nullptr), parent(nullptr), color(Color::RED){}

RBTree::RBTree() : root(nullptr) {}

RBTree::RBTree(std::initializer_list<int> list)
    : root(nullptr)
{
    for (int key : list) {
        insert(key);
    }
}

RBTree::~RBTree()
{
    std::stack<Node*> nodes;
    if (root) nodes.push(root);
    while (!nodes.empty()) {
        Node* node = nodes.top();
        nodes.pop();
        if (node->left) nodes.push(node->left);
        if (node->right) nodes.push(node->right);
        delete node;
    }
}

bool isRed(Node* node)
{
    return node && node->color == Color::RED;
}

void flipColors(Node* node)
{
    node->color = (node->color == Color::RED) ? Color::BLACK : Color::RED;
    node->left->color = (node->left->color == Color::RED) ? Color::BLACK : Color::RED;
    node->right->color = (node->right->color == Color::RED) ? Color::BLACK : Color::RED;
}

Node* rotateLeft(Node* node)
{
    Node* right_child = node->right;
    node->right = right_child->left;
    if (right_child->left) right_child->left->parent = node;
    right_child->left = node;
    right_child->parent = node->parent;
    node->parent = right_child;
    std::swap(right_child->color, node->color);

    node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0);
    right_child->size = 1 + (right_child->left ? right_child->left->size : 0) + (right_child->right ? right_child->right->size : 0);

    return right_child;
}

Node* rotateRight(Node* node)
{
    Node* left_child = node->left;
    node->left = left_child->right;
    if (left_child->right) left_child->right->parent = node;
    left_child->right = node;
    left_child->parent = node->parent;
    node->parent = left_child;
    std::swap(left_child->color, node->color);

    node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0);
    left_child->size = 1 + (left_child->left ? left_child->left->size : 0) + (left_child->right ? left_child->right->size : 0);

    return left_child;
}

Node* balance(Node* node)
{
    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0);

    int left_height = node->left ? node->left->height : 0;
    int right_height = node->right ? node->right->height : 0;
    if (isRed(node)) {
        node->height = std::max(left_height, right_height);
    } else {
        node->height = 1 + std::max(left_height, right_height);
    }
    return node;
}

Node* insert(Node* node, int key, Node* parent)
{
    if (!node) {
        Node* new_node = new Node(key);
        new_node->parent = parent;
        return new_node;
    }

    if (key < node->key) {
        node->left = insert(node->left, key, node);
    } else if (key > node->key) {
        node->right = insert(node->right, key, node);
    } else {
        return node;
    }

    return balance(node);
}

void RBTree::insert(int key)
{
    root = ::insert(root, key, nullptr);
    if (root) {
        root->color = Color::BLACK;
    }
}

int* RBTree::find(int key)
{
    Node* cur_node = root;
    while (cur_node) {
        if (key < cur_node->key) {
            cur_node = cur_node->left;
        } else if (key > cur_node->key) {
            cur_node = cur_node->right;
        } else {
            return &(cur_node->key);
        }
    }
    return nullptr;
}

int RBTree::size() const
{
    return root ? root->size : 0;
}

int* RBTree::lowerBound(int key)
{
    Node* cur_node = root;
    Node* best_node = nullptr;
    while (cur_node) {
        if (key <= cur_node->key) {
            best_node = cur_node;
            cur_node = cur_node->left;
        } else {
            cur_node = cur_node->right;
        }
    }
    return best_node ? &(best_node->key) : nullptr;
}

bool RBTree::empty() const
{
    return root == nullptr;
}

int RBTree::height() const
{
    return root ? root->height : 0;
}