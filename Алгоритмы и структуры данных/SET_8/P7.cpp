#include <iostream>
#include <vector>
#include <string>
#include <queue>

struct TrieNode {
    TrieNode* child[26];
    TrieNode* suffixLink;
    TrieNode* outputLink;
    std::vector<int> patternInds;
    
    TrieNode() {
        for (int i = 0; i < 26; ++i) {
            child[i] = nullptr;
        }
        suffixLink = nullptr;
        outputLink = nullptr;
    }
};

TrieNode* add_node() {
    TrieNode* temp = new TrieNode();
    return temp;
}

void build_Trie(TrieNode* root, std::vector<std::string> &patterns) {
    for (size_t i = 0; i < patterns.size(); ++i) {
        TrieNode* cur = root;
        for (char c : patterns[i]) {
            int idx = c - 'a';
            if (!cur->child[idx]) {
                cur->child[idx] = add_node();
            }
            cur = cur->child[idx];
        }
        cur->patternInds.push_back(i);
    }
}

void build_suffix_and_output_links(TrieNode* root) {
    root->suffixLink = root;
    std::queue<TrieNode*> qu;
    
    for (int i = 0; i < 26; ++i) {
        if (root->child[i]) {
            root->child[i]->suffixLink = root;
            qu.push(root->child[i]);
        }
    }
    
    while (!qu.empty()) {
        TrieNode* curState = qu.front();
        qu.pop();
        
        for (int i = 0; i < 26; ++i) {
            if (!curState->child[i]) continue;
            
            TrieNode* temp = curState->suffixLink;
            while (temp != root && !temp->child[i]) {
                temp = temp->suffixLink;
            }
            
            if (temp->child[i]) {
                curState->child[i]->suffixLink = temp->child[i];
            } else {
                curState->child[i]->suffixLink = root;
            }
            
            qu.push(curState->child[i]);
        }
        
        if (!curState->suffixLink->patternInds.empty()) {
            curState->outputLink = curState->suffixLink;
        } else if (curState->suffixLink->outputLink != nullptr) {
            curState->outputLink = curState->suffixLink->outputLink;
        }
    }
}

void search_pattern(TrieNode* root, std::string &text, std::vector<std::vector<int>> &indices) {
    TrieNode* parent = root;
    
    for (size_t i = 0; i < text.length(); ++i) {
        int idx = text[i] - 'a';
        
        while (parent != root && !parent->child[idx]) {
            parent = parent->suffixLink;
        }
        
        if (parent->child[idx]) {
            parent = parent->child[idx];
        }

        for (int patternInd : parent->patternInds) {
            indices[patternInd].push_back(i);
        }
        
        TrieNode* temp = parent->outputLink;
        while (temp != nullptr) {
            for (int patternInd : temp->patternInds) {
                indices[patternInd].push_back(i);
            }
            temp = temp->outputLink;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string text;
    std::cin >> text;
    
    int n;
    std::cin >> n;
    
    std::vector<std::string> patterns(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> patterns[i];
    }
    
    TrieNode* root = add_node();
    build_Trie(root, patterns);
    build_suffix_and_output_links(root);
    
    std::vector<std::vector<int>> endPositions(n);
    search_pattern(root, text, endPositions);
    
    for (int i = 0; i < n; ++i) {
        std::cout << endPositions[i].size();
        for (int endPos : endPositions[i]) {
            int startPos = endPos - (int)patterns[i].length() + 2;
            std::cout << " " << startPos;
        }
        std::cout << "\n";
    }
    
    return 0;
}