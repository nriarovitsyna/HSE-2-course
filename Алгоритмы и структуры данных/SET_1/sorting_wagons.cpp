#include <iostream>
#include <vector>
#include <stack>

int main() {
    int n;
    std::cin >> n;
    
    std::vector<int> wagons(n);
    for (int i = 0; i < n; i++) {
        std::cin >> wagons[i];
    }
    
    std::stack<int> stack;
    std::vector<std::pair<int, int>> actions;
    int cur = 1;
    int idx = 0;
    
    while (cur <= n) {
        if (!stack.empty() && stack.top() == cur) {
            int cnt = 0;
            
            while (!stack.empty() && stack.top() == cur) {
                stack.pop();
                cnt++;
                cur++;
            }

            actions.push_back(std::make_pair(2, cnt));
        } else if (idx < n) {
            int start_idx = idx;
            bool flag = false;
            
            while (idx < n && !flag) {
                stack.push(wagons[idx]);
                if (wagons[idx] == cur) {
                    flag = true;
                }

                idx++;
            }
            
            int cnt = idx - start_idx;
            actions.push_back(std::make_pair(1, cnt));
            
            if (!flag && idx >= n) {
                std::cout << 0 << '\n';
                return 0;
            }
        } else {
            std::cout << 0 << '\n';
            return 0;
        }
    }
    
    for (auto& act : actions) {
        std::cout << act.first << " " << act.second << "\n";
    }
    
    return 0;
}