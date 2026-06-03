#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

class TwoSAT {
private:
    int n;
    int size;
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> rev_graph;
    
    int vertex(int idx, bool val) {
        return 2 * idx + (val ? 1 : 0);
    }
    
    int opposite(int v) {
        return v ^ 1;
    }
    
    void DFS_1(int v, std::vector<bool>& visited, std::stack<int>& dead_ends) {
        visited[v] = true;
        for (int u : graph[v]) {
            if (!visited[u]) {
                DFS_1(u, visited, dead_ends);
            }
        }
        dead_ends.push(v);
    }
    
    void DFS_2(int v, std::vector<int>& groups, int cnt_group) {
        groups[v] = cnt_group;
        for (int u : rev_graph[v]) {
            if (groups[u] == -1) {
                DFS_2(u, groups, cnt_group);
            }
        }
    }
    
public:
    TwoSAT(int variables) : n(variables), size(2 * variables) {
        graph.resize(size);
        rev_graph.resize(size);
    }
    
    void add_implication(int A, int B) {
        graph[A].push_back(B);
        rev_graph[B].push_back(A);
    }
    
    void add_clause(int i1, bool e1, int i2, bool e2) {
        add_implication(opposite(vertex(i1, e1)), vertex(i2, e2));
        add_implication(opposite(vertex(i2, e2)), vertex(i1, e1));
    }
    
    std::vector<bool> find_SCC() {
        std::vector<bool> visited(size, false);
        std::stack<int> dead_ends;
        
        for (int i = 0; i < size; ++i) {
            if (!visited[i]) {
                DFS_1(i, visited, dead_ends);
            }
        }
        
        std::vector<int> groups(size, -1);
        int cnt_group = 0;
        
        while (!dead_ends.empty()) {
            int v = dead_ends.top();
            dead_ends.pop();
            if (groups[v] == -1) {
                DFS_2(v, groups, ++cnt_group);
            }
        }

        for (int i = 0; i < n; ++i) {
            if (groups[vertex(i, false)] == groups[vertex(i, true)]) {
                return std::vector<bool>();
            }
        }

        std::vector<bool> result(n);
        for (int i = 0; i < n; ++i) {
            result[i] = (groups[vertex(i, false)] < groups[vertex(i, true)]);
        }
        
        return result;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    while (std::cin >> n >> m) {
        TwoSAT solver(n);
        
        for (int i = 0; i < m; ++i) {
            int i1, e1, i2, e2;
            std::cin >> i1 >> e1 >> i2 >> e2;
            solver.add_clause(i1, e1, i2, e2);
        }
        
        std::vector<bool> result = solver.find_SCC();
        
        for (bool val : result) {
            std::cout << (val ? '1' : '0');
        }
        std::cout << "\n";
    }
    
    return 0;
}