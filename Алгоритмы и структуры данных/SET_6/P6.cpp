#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

class FloydWarshall {
private:
    int n;
    std::vector<std::vector<long long>> dist;
    const long long INF = std::numeric_limits<long long>::max() / 2;
    
public:
    FloydWarshall(int vertices) : n(vertices) {
        dist.assign(n, std::vector<long long>(n, INF));
        
        for (int i = 0; i < n; ++i) {
            dist[i][i] = 0;
        }
    }
    
    void addEdge(int u, int v, long long w) {
        if (w < dist[u][v]) {
            dist[u][v] = w;
        }
    }
    
    void floydWarshallAPSP() {
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                if (dist[i][k] < INF) {
                    for (int j = 0; j < n; ++j) {
                        if (dist[k][j] < INF) {
                            if (dist[i][j] > dist[i][k] + dist[k][j]) {
                                dist[i][j] = dist[i][k] + dist[k][j];
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            if (dist[i][i] < 0) {
                for (int j = 0; j < n; ++j) {
                    for (int k = 0; k < n; ++k) {
                        if (dist[j][i] < INF && dist[i][k] < INF) {
                            dist[j][k] = -INF;
                        }
                    }
                }
            }
        }
    }
    
    void print() {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (u == v) continue;
                
                if (dist[u][v] >= INF) {
                    std::cout << u << " " << v << " -1\n";
                } else if (dist[u][v] <= -INF) {
                    std::cout << u << " " << v << " -inf\n";
                } else {
                    std::cout << u << " " << v << " " << dist[u][v] << "\n";
                }
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    FloydWarshall fw(n);
    
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        fw.addEdge(u, v, w);
    }
    
    fw.floydWarshallAPSP();
    fw.print();
    
    return 0;
}