#include <iostream>
#include <vector>
#include <string>
#include <climits>

struct Node {
    int key;
    int left;
    int right;
    char color;
};

bool isBST(const std::vector<Node>& tree, int root, long long min_val, long long max_val) {
    if (root == 0) return true;
    if (tree[root].key <= min_val || tree[root].key >= max_val) return false;
    return isBST(tree, tree[root].left, min_val, tree[root].key) &&
           isBST(tree, tree[root].right, tree[root].key, max_val);
}

std::pair<bool, int> isRB(const std::vector<Node>& tree, int node, bool& is_valid) {
    if (!is_valid) return std::make_pair(false, 0);
    
    if (node == 0) return std::make_pair(true, 1);
    
    if (tree[node].color == 'R') {
        if (tree[node].left != 0 && tree[tree[node].left].color == 'R') {
            is_valid = false;
            return std::make_pair(false, 0);
        }
        if (tree[node].right != 0 && tree[tree[node].right].color == 'R') {
            is_valid = false;
            return std::make_pair(false, 0);
        }
    }
    
    auto left_check = isRB(tree, tree[node].left, is_valid);
    auto right_check = isRB(tree, tree[node].right, is_valid);
    
    if (!left_check.first || !right_check.first) {
        is_valid = false;
        return std::make_pair(false, 0);
    }
    
    if (left_check.second != right_check.second) {
        is_valid = false;
        return std::make_pair(false, 0);
    }
    
    int black_height = left_check.second;
    if (tree[node].color == 'B') {
        black_height++;
    }
    
    return std::make_pair(true, black_height);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, root;
    
    while (std::cin >> n) {
        if (n == 0) {
            std::cout << "NO\n";
            continue;
        }
        
        std::cin >> root;
        
        std::vector<Node> tree(n + 1);
        std::vector<int> parent(n + 1, 0);
        
        for (int i = 0; i < n; i++) {
            int vert_num, key, left, right;
            std::string color_data;
            char color;
            
            std::cin >> vert_num >> key;
            
            std::string left_data, right_data;
            std::cin >> left_data >> right_data;
            
            left = (left_data == "null" ? 0 : std::stoi(left_data));
            right = (right_data == "null" ? 0 : std::stoi(right_data));
            
            std::cin >> color_data;
            color = color_data[0];
            
            tree[vert_num] = {key, left, right, color};
            
            if (left != 0) parent[left] = vert_num;
            if (right != 0) parent[right] = vert_num;
        }
        
        if (tree[root].color != 'B') {
            std::cout << "NO\n";
            continue;
        }
        
        if (!isBST(tree, root, LLONG_MIN, LLONG_MAX)) {
            std::cout << "NO\n";
            continue;
        }

        bool is_valid = true;
        auto result = isRB(tree, root, is_valid);
        
        if (is_valid && result.first) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    
    return 0;
}