#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct Edge {
    int from;
    int to;
    int weight;
    
    Edge(int u, int v, int w) : from(u), to(v), weight(w) {}
};

class BellmanFord {
private:
    int n;
    std::vector<Edge> edges;
    std::vector<long long> dist;
    std::vector<int> parent;
    
    const long long INF = std::numeric_limits<long long>::max() / 2;
    
public:
    BellmanFord(int vertices) : n(vertices) {
        dist.resize(n, INF);
        parent.resize(n, -1);
    }
    
    void addEdge(int u, int v, int w) {
        edges.emplace_back(u, v, w);
    }
    
    std::vector<long long> findShortestPaths(int start) {
        dist[start] = 0;
        
        for (int i = 1; i < n; ++i) {
            bool improved = false;
            for (const Edge& e : edges) {
                if (dist[e.from] < INF) {
                    if (dist[e.to] > dist[e.from] + e.weight) {
                        dist[e.to] = dist[e.from] + e.weight;
                        parent[e.to] = e.from;
                        improved = true;
                    }
                }
            }

            if (!improved) break;
        }
        
        std::vector<bool> neg_cycle(n, false);

        for (int i = 0; i < n; ++i) {
            for (const Edge& e : edges) {
                if (dist[e.from] < INF) {
                    if (dist[e.to] > dist[e.from] + e.weight) {
                        dist[e.to] = dist[e.from] + e.weight;
                        neg_cycle[e.to] = true;
                    }
                }
            }
        }
        
        for (int i = 0; i < n; ++i) {
            for (const Edge& e : edges) {
                if (neg_cycle[e.from]) {
                    neg_cycle[e.to] = true;
                }
            }
        }
        
        std::vector<long long> result(n);
        for (int i = 0; i < n; ++i) {
            if (i == start) continue;
            
            if (neg_cycle[i] || dist[i] <= -INF) {
                result[i] = -INF;
            } else {
                result[i] = dist[i];
            }
        }
        
        return result;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    BellmanFord bf(n);
    
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        bf.addEdge(u, v, w);
    }
    
    std::vector<long long> result = bf.findShortestPaths(0);

    for (int i = 1; i < n; ++i) {
        if (result[i] == -std::numeric_limits<long long>::max() / 2) {
            std::cout << "-inf\n";
        } else {
            std::cout << result[i] << "\n";
        }
    }
    
    return 0;
}