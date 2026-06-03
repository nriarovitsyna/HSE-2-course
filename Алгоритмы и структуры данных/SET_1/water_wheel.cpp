#include <iostream>
#include <vector>

int get_cycles(const std::vector<int>& a, int n) {
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) {
            bool flag = true;
            
            for (int j = 0; j < n; j++) {
                if (a[j] != a[j % i]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                return i;
            }
        }
    }

    return n;
}

int main() {
    int n;
    std::cin >> n;

    std::vector<int> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }
    
    int max_dist = 0;
    int max_cnt = 0;
    int cycles = get_cycles(a, n);

    for (int i = 0; i < cycles; i++) {
    int cur = 0;
        
        for (int j = 0; j < n; j++) {
            if (a[j] != a[(j - i + n) % n]) {
                cur++;
            }
        }
        
        if (cur > max_dist) {
            max_dist = cur;
            max_cnt = 1;
        } else if (cur == max_dist) {
            max_cnt++;
        }
    }
    
    std::cout << max_cnt << '\n';
    
    return 0;
}