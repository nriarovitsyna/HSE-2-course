#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

struct Node {
    char ch;
    int freq;
    int order;
    Node* left;
    Node* right;
    
    Node(char c, int f, int ord) : ch(c), freq(f), order(ord), left(nullptr), right(nullptr) {}
    Node(int f, Node* l, Node* r, int ord) : ch('\0'), freq(f), order(ord), left(l), right(r) {}
    
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        if (a->freq != b->freq) {
            return a->freq > b->freq;
        }

        if (a->isLeaf() != b->isLeaf()) {
            return !a->isLeaf();
        }

        if (a->isLeaf() && b->isLeaf()) {
            return a->ch > b->ch;
        }

        return a->order > b->order;
    }
};

void buildCodes(Node* root, const std::string& code, std::unordered_map<char, std::string>& codes) {
    if (!root) return;
    
    if (root->isLeaf()) {
        codes[root->ch] = code;
        return;
    }
    
    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string s;
    std::cin >> s;
    
    std::unordered_map<char, int> freq;
    for (char c : s) {
        freq[c]++;
    }
    
    int k = freq.size();
    
    if (k == 1) {
        char ch = s[0];
        int len = s.length();
        std::cout << k << " " << len << "\n";
        std::cout << ch << ": 0\n";
        std::string encoded(len, '0');
        std::cout << encoded << "\n";
        return 0;
    }
    
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    
    int orderCounter = 0;
    std::vector<std::pair<char, int>> sortedFreq(freq.begin(), freq.end());
    std::sort(sortedFreq.begin(), sortedFreq.end());
    
    for (auto& p : sortedFreq) {
        pq.push(new Node(p.first, p.second, orderCounter++));
    }
    
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        
        Node* parent = new Node(left->freq + right->freq, left, right, orderCounter++);
        pq.push(parent);
    }
    
    Node* root = pq.top();
    
    std::unordered_map<char, std::string> codes;
    buildCodes(root, "", codes);
    
    long long encodedLength = 0;
    for (char c : s) {
        encodedLength += codes[c].length();
    }
    
    std::cout << k << " " << encodedLength << "\n";
    
    std::vector<char> sortedChars;
    for (auto& p : freq) {
        sortedChars.push_back(p.first);
    }
    std::sort(sortedChars.begin(), sortedChars.end());
    
    for (char c : sortedChars) {
        std::cout << c << ": " << codes[c] << "\n";
    }
    
    for (char c : s) {
        std::cout << codes[c];
    }
    std::cout << "\n";
    
    deleteTree(root);
    
    return 0;
}