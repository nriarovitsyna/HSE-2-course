#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Edge {
    int to;
    int cap;
    int rev;
};

class DinicNetwork {
public:
    int num_vert;
    std::vector<std::vector<Edge>> adj;
    std::vector<int> level;
    std::vector<size_t> ptr;

    DinicNetwork(int n) : num_vert(n), adj(n), level(n), ptr(n) {}

    void addEdge(int from, int to, int cap) {
        Edge forward = {to, cap, (int)adj[to].size()};
        Edge backward = {from, 0, (int)adj[from].size()};
        adj[from].push_back(forward);
        adj[to].push_back(backward);
    }

    bool BFS(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        std::queue<int> bfs_queue;
        level[source] = 0;
        bfs_queue.push(source);

        while (!bfs_queue.empty()) {
            int vert = bfs_queue.front();
            bfs_queue.pop();

            for (const Edge& e : adj[vert]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[vert] + 1;
                    bfs_queue.push(e.to);
                }
            }
        }
        return level[sink] != -1;
    }

    int DFS(int cur_v, int sink, int flow) {
        if (cur_v == sink)
            return flow;

        for (; ptr[cur_v] < adj[cur_v].size(); ++ptr[cur_v]) {
            Edge& e = adj[cur_v][ptr[cur_v]];

            if (e.cap > 0 && level[e.to] == level[cur_v] + 1) {
                int f = DFS(e.to, sink, std::min(flow, e.cap));
                if (f > 0) {
                    e.cap -= f;
                    adj[e.to][e.rev].cap += f;
                    return f;
                }
            }
        }
        return 0;
    }

    int maxFlow(int source, int sink) {
        int flow = 0;
        while (true) {
            if (!BFS(source, sink))
                break;

            fill(ptr.begin(), ptr.end(), 0);

            while (int added = DFS(source, sink, INT32_MAX)) {
                flow += added;
            }
        }
        return flow;
    }
};

int main() {
    int n, m, a, b;
    std::cin >> n >> m >> a >> b;

    std::vector<std::string> floor(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> floor[i];
    }

    if (a >= 2 * b) {
        int free = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (floor[i][j] == '*') {
                    free++;
                }
            }
        }
        std::cout << free * b << '\n';
        return 0;
    }

    int source = n * m;
    int sink = source + 1;
    DinicNetwork network(sink + 1);

    auto getNodeId = [m](int i, int j) {
        return i * m + j;
    };

    int move_row[] = {0, 1, 0, -1};
    int move_col[] = {1, 0, -1, 0};

    int total_free = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (floor[i][j] == '*') {
                total_free++;
            }
        }
    }

    if (total_free == 0) {
        std::cout << 0 << '\n';
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (floor[i][j] == '*') {
                int node = getNodeId(i, j);

                if ((i + j) % 2 == 0) {
                    network.addEdge(source, node, 1);
                    
                    for (int step = 0; step < 4; ++step) {
                        int new_row = i + move_row[step];
                        int new_col = j + move_col[step];
                        if (new_row >= 0 && new_row < n && new_col >= 0 && new_col < m && floor[new_row][new_col] == '*') {
                            int neighbor = getNodeId(new_row, new_col);
                            network.addEdge(node, neighbor, 1);
                        }
                    }
                } else {
                    network.addEdge(node, sink, 1);
                }
            }
        }
    }

    int max_matching = network.maxFlow(source, sink);

    int result = max_matching * a + (total_free - 2 * max_matching) * b;
    std::cout << result << '\n';

    return 0;
}