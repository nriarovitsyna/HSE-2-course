#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

const long long INF = pow(2, 60);

struct Edge {
    int to;
    long long cap;
    int rev;
};

std::vector<std::vector<Edge>> G;
std::vector<int> level, iter;

void add_edge(int from, int to, long long cap) {
    G[from].push_back({to, cap, (int)G[to].size()});
    G[to].push_back({from, 0, (int)G[from].size() - 1});
}

void BFS(int start) {
    std::fill(level.begin(), level.end(), -1);

    std::queue<int> vert;
    level[start] = 0;
    vert.push(start);

    while (!vert.empty()) {
        int cur = vert.front(); vert.pop();

        for (auto &edge : G[cur]) {
            if (edge.cap > 0 && level[edge.to] < 0) {
                level[edge.to] = level[cur] + 1;
                vert.push(edge.to);
            }
        }
    }
}

long long DFS(int start, int end, long long flow) {
    if (start == end) return flow;

    for (int &i = iter[start]; i < (int)G[start].size(); i++) {
        Edge &edge = G[start][i];

        if (edge.cap > 0 && level[start] < level[edge.to]) {
            long long path_flow = DFS(edge.to, end, std::min(flow, edge.cap));
            if (path_flow > 0) {
                edge.cap -= path_flow;
                G[edge.to][edge.rev].cap += path_flow;
                return path_flow;
            }
        }
    }
    return 0;
}

long long max_flow(int start, int end) {
    long long total_flow = 0;

    for (;;) {
        BFS(start);
        
        if (level[end] < 0) return total_flow;

        std::fill(iter.begin(), iter.end(), 0);
        long long cur_flow;

        while ((cur_flow = DFS(start, end, INF)) > 0) {
            total_flow += cur_flow;
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;

    G.resize(n + 1);
    level.resize(n + 1);
    iter.resize(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long c;
        std::cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    std::cout << max_flow(1, n) << '\n';
    return 0;
}