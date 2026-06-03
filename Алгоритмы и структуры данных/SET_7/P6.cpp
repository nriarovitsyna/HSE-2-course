#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

const long long INF = 4LL;

struct Edge {
    int to;
    long long cap;
    int rev;
    bool is_orig;
    Edge(int t, long long c, int r, bool o) : to(t), cap(c), rev(r), is_orig(o) {}
};

class Dinic {
public:
    int n;
    std::vector<std::vector<Edge>> G;
    std::vector<int> level, ptr;

    Dinic(int num) : n(num), G(num), level(num), ptr(num) {}

    void add_edge(int from, int to, long long cap) {
        G[from].push_back({to, cap, (int)G[to].size(), true});
        G[to].push_back({from, 0LL, (int)G[from].size() - 1, false});
    }

    bool BFS(int start, int end) {
        std::fill(level.begin(), level.end(), -1);
        level[start] = 0;
        std::queue<int> bfs_queue;
        bfs_queue.push(start);

        while (!bfs_queue.empty()) {
            int v = bfs_queue.front(); bfs_queue.pop();
            for (const auto& e : G[v]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    bfs_queue.push(e.to);
                }
            }
        }
        return level[end] != -1;
    }

    long long DFS(int start, int end, long long flow, std::vector<int>& path) {
        if (start == end) {
            path.push_back(end);
            return flow;
        }

        for (int &i = ptr[start]; i < (int)G[start].size(); ++i) {
            Edge& edge = G[start][ptr[start]];
            if (edge.cap > 0 && level[edge.to] == level[start] + 1) {
                std::vector<int> sub_path;
                long long pushed = DFS(edge.to, end, std::min(flow, edge.cap), sub_path);
                if (pushed > 0) {
                    edge.cap -= pushed;
                    G[edge.to][edge.rev].cap += pushed;
                    path = std::move(sub_path);
                    path.push_back(start);
                    return pushed;
                }
            }
        }
        return 0;
    }

    std::vector<std::vector<int>> find_paths(int start, int end) {
        std::vector<std::vector<int>> paths;
        long long total = 0;

        while (total < 2 && BFS(start, end)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (total < 2) {
                std::vector<int> path;
                long long added = DFS(start, end, INF, path);
                if (added == 0) break;
                total += added;
                reverse(path.begin(), path.end());
                paths.push_back(std::move(path));
            }
        }

        return paths;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, a, h;
    std::cin >> n >> m >> a >> h;
    a--; h--;

    Dinic dinic(n);

    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        x--; y--;
        dinic.add_edge(x, y, 1);
    }

    auto paths = dinic.find_paths(a, h);

    if (paths.size() < 2) {
        std::cout << "NO\n";
        return 0;
    }

    auto restore_true_paths = [&](int start, int end) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> result;
        for (int k = 0; k < 2; ++k) {
            std::vector<int> parent(n, -1);
            std::queue<int> vert;
            vert.push(start);
            parent[start] = start;
            bool found = false;
            while (!vert.empty() && !found) {
                int v = vert.front(); vert.pop();
                for (auto &e : dinic.G[v]) {
                    if (e.is_orig && e.cap == 0 && parent[e.to] == -1) {
                        parent[e.to] = v;
                        if (e.to == end) {
                            found = true;
                            break;
                        }
                        vert.push(e.to);
                    }
                }
            }
            if (!found) break;

            std::vector<int> path;
            int cur = end;
            while (cur != start) {
                path.push_back(cur);
                int previous = parent[cur];
                for (auto &e : dinic.G[previous]) {
                    if (e.is_orig && e.to == cur && e.cap == 0) {
                        e.cap = -1;
                        break;
                    }
                }
                cur = previous;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            result.push_back(path);
        }
        return result;
    };

    auto true_paths = restore_true_paths(a, h);

    std::cout << "YES\n";
    for (const auto &p : true_paths) {
        for (int v : p) std::cout << v + 1 << " ";
        std::cout << "\n";
    }

    return 0;
}