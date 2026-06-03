#include <iostream>
#include <vector>
#include <algorithm>

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

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
};

int Kraskal_algorithm(int n, std::vector<Edge>& edges, int skip_edge = -1) {
    UnionFind uf(n);
    int total_weight = 0;
    int used_edges = 0;
    
    for (int i = 0; i < edges.size(); ++i) {
        if (i == skip_edge) continue;
        
        if (uf.unite(edges[i].u, edges[i].v)) {
            total_weight += edges[i].weight;
            used_edges++;
        }
    }
    
    if (used_edges != n - 1) return -1;
    return total_weight;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    std::vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> edges[i].u >> edges[i].v >> edges[i].weight;
        --edges[i].u;
        --edges[i].v;
    }
    
    std::sort(edges.begin(), edges.end());
    
    UnionFind uf_MST(n);
    std::vector<int> MST;
    int S1 = 0;
    
    for (int i = 0; i < edges.size(); ++i) {
        if (uf_MST.unite(edges[i].u, edges[i].v)) {
            S1 += edges[i].weight;
            MST.push_back(i);
        }
    }
    
    int S2 = 1e9;
    
    for (int skip : MST) {
        int cur_weight = Kraskal_algorithm(n, edges, skip);
        if (cur_weight != -1 && cur_weight >= S1) {
            S2 = std::min(S2, cur_weight);
        }
    }
    
    std::cout << S1 << " " << S2 << "\n";
    
    return 0;
}