#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> size;
    
public:
    UnionFind(int n) {
        parent.resize(n);
        size.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }
    
    int find(int A) {
        if (parent[A] != A) {
            parent[A] = find(parent[A]);
        }
        return parent[A];
    }
    
    bool unite(int A, int B) {
        int rootA = find(A);
        int rootB = find(B);
        if (rootA == rootB) return false;
        
        if (size[rootA] < size[rootB]) {
            parent[rootA] = rootB;
        } else if (size[rootA] > size[rootB]) {
            parent[rootB] = rootA;
        } else {
            parent[rootB] = rootA;
            size[rootA]++;
        }
        return true;
    }
    
    bool connected(int A, int B) {
        return find(A) == find(B);
    }
};

struct Operation {
    std::string type;
    int u, v;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m, k;
    std::cin >> n >> m >> k;
    
    std::vector<std::pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
        --edges[i].first;
        --edges[i].second;
    }
    
    std::vector<Operation> operations(k);
    std::map<std::pair<int, int>, bool> cut_edges;
    
    for (int i = 0; i < k; ++i) {
        std::cin >> operations[i].type >> operations[i].u >> operations[i].v;
        --operations[i].u;
        --operations[i].v;
        
        if (operations[i].type == "cut") {
            int A = operations[i].u, B = operations[i].v;
            if (A > B) std::swap(A, B);
            cut_edges[{A, B}] = true;
        }
    }

    UnionFind uf(n);
    
    for (int i = 0; i < m; ++i) {
        int A = edges[i].first, B = edges[i].second;
        if (A > B) std::swap(A, B);
        
        if (!cut_edges[{A, B}]) {
            uf.unite(A, B);
        }
    }
    
    std::vector<std::string> results;
    
    for (int i = k - 1; i >= 0; --i) {
        if (operations[i].type == "ask") {
            if (uf.connected(operations[i].u, operations[i].v)) {
                results.push_back("YES");
            } else {
                results.push_back("NO");
            }
        } else {
            uf.unite(operations[i].u, operations[i].v);
        }
    }
    
    for (int i = results.size() - 1; i >= 0; --i) {
        std::cout << results[i] << "\n";
    }
    
    return 0;
}