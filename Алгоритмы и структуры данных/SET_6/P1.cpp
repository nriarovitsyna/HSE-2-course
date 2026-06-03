#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

void DFS_1(int v, const std::vector<std::vector<int>>& graph, std::vector<bool>& visited, std::stack<int>& dead_ends) {
    visited[v] = true;
    for (int u : graph[v]) {
        if (!visited[u]) {
            DFS_1(u, graph, visited, dead_ends);
        }
    }
    dead_ends.push(v);
}

void DFS_2(int v, const std::vector<std::vector<int>>& rev_graph, std::vector<int>& groups, int cnt_group) {
    groups[v] = cnt_group;
    for (int u : rev_graph[v]) {
        if (groups[u] == -1) {
            DFS_2(u, rev_graph, groups, cnt_group);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> graph(n), rev_graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        --u; --v;
        graph[u].push_back(v);
        rev_graph[v].push_back(u);
    }

    std::vector<bool> visited(n, false);
    std::stack<int> dead_ends;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            DFS_1(i, graph, visited, dead_ends);
        }
    }

    std::vector<int> groups(n, -1);
    int cnt_group = 0;
    while (!dead_ends.empty()) {
        int v = dead_ends.top();
        dead_ends.pop();
        if (groups[v] == -1) {
            DFS_2(v, rev_graph, groups, ++cnt_group);
        }
    }

    std::cout << cnt_group << "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << groups[i] << " ";
    }
    std::cout << "\n";

    return 0;
}