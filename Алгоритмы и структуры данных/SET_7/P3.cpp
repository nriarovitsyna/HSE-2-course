#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

struct Edge {
    int to;
    int rev;
    bool used;
};

std::vector<std::vector<Edge>> G;
std::unordered_map<std::string, int> name_to_id;
int total_vertices = 0;

int get_id(const std::string& s) {
    auto it = name_to_id.find(s);
    if (it != name_to_id.end()) return it->second;
    name_to_id[s] = total_vertices++;
    G.emplace_back();
    return total_vertices - 1;
}

bool BFS(int start, int end, std::vector<int>& parent, std::vector<int>& parent_edge) {
    parent.assign(total_vertices, -1);
    parent[start] = -2;
    
    std::queue<int> vert;
    vert.push(start);
    
    while (!vert.empty()) {
        int v = vert.front(); vert.pop();
        
        for (size_t i = 0; i < G[v].size(); ++i) {
            Edge& edge = G[v][i];
            if (!edge.used && parent[edge.to] == -1) {
                parent[edge.to] = v;
                parent_edge[edge.to] = i;
                if (edge.to == end) return true;
                vert.push(edge.to);
            }
        }
    }
    return false;
}

int max_flow(int start, int end) {
    if (start == end) return 0;
    
    int flow = 0;
    std::vector<int> parent(total_vertices);
    std::vector<int> parent_edge(total_vertices);
    
    for (int i = 0; i < total_vertices; ++i) {
        for (Edge& edge : G[i]) {
            edge.used = false;
        }
    }
    
    while (BFS(start, end, parent, parent_edge)) {
        int v = end;
        while (v != start) {
            int p = parent[v];
            int e_idx = parent_edge[v];
            
            G[p][e_idx].used = true;
            
            for (size_t i = 0; i < G[v].size(); ++i) {
                if (G[v][i].to == p) {
                    G[v][i].used = true;
                    break;
                }
            }
            v = p;
        }
        ++flow;
    }
    return flow;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m, k;
    std::cin >> n >> m >> k;
    
    G.reserve(n * 2);
    name_to_id.reserve(n * 2);
    
    for (int i = 0; i < m; ++i) {
        std::string A, B;
        std::cin >> A >> B;
        int u = get_id(A);
        int v = get_id(B);
        
        G[u].push_back({v, (int)G[v].size(), false});
        G[v].push_back({u, (int)G[u].size() - 1, false});
    }
    
    std::vector<std::pair<int, int>> queries(k);
    for (int i = 0; i < k; ++i) {
        std::string A, B;
        std::cin >> A >> B;
        
        auto it1 = name_to_id.find(A);
        auto it2 = name_to_id.find(B);
        
        if (it1 == name_to_id.end() || it2 == name_to_id.end()) {
            queries[i] = {-1, -1};
        } else {
            queries[i] = {it1->second, it2->second};
        }
    }
    
    for (int i = 0; i < k; ++i) {
        int u = queries[i].first;
        int v = queries[i].second;
        
        if (u == -1 || v == -1 || u == v) {
            std::cout << "0\n";
            continue;
        }
        
        std::cout << max_flow(u, v) << '\n';
    }
    
    return 0;
}