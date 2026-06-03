#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <cstring>

std::vector<std::vector<int>> adj;
std::vector<int> match;
std::vector<bool> used;

bool DFS(int vert) {
    if (used[vert]) return false;
    used[vert] = true;
    
    for (int to : adj[vert]) {
        if (match[to] == -1 || DFS(match[to])) {
            match[to] = vert;
            return true;
        }
    }
    return false;
}

int main() {
    int N;
    std::cin >> N;
    
    std::vector<std::set<std::string>> ingredients(N);
    for (int i = 0; i < N; i++) {
        int k;
        std::cin >> k;
        for (int j = 0; j < k; j++) {
            std::string comp;
            std::cin >> comp;
            ingredients[i].insert(comp);
        }
    }
    
    adj.resize(N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) continue;
            
            bool is_subset = true;
            for (const std::string& ing : ingredients[i]) {
                if (ingredients[j].find(ing) == ingredients[j].end()) {
                    is_subset = false;
                    break;
                }
            }
            
            if (is_subset) {
                adj[i].push_back(j);
            }
        }
    }
    
    match.assign(N, -1);
    
    int size = 0;
    for (int i = 0; i < N; i++) {
        used.assign(N, false);
        if (DFS(i)) {
            size++;
        }
    }
    
    std::cout <<  N - size << '\n';
    
    return 0;
}