#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    
    std::vector<int> result(n);
    
    if (n % 6 == 2) {
        int idx = 0;
        for (int i = 2; i <= n; i += 2) {
            result[idx++] = i;
        }
        result[idx++] = 3;
        result[idx++] = 1;
        for (int i = 7; i <= n; i += 2) {
            result[idx++] = i;
        }
        if (n >= 5) {
            result[idx++] = 5;
        }
    } 
    else if (n % 6 == 3) {
        int idx = 0;
        for (int i = 4; i <= n; i += 2) {
            result[idx++] = i;
        }
        result[idx++] = 2;
        for (int i = 5; i <= n; i += 2) {
            result[idx++] = i;
        }
        result[idx++] = 1;
        result[idx++] = 3;
    } 
    else {
        int idx = 0;
        for (int i = 2; i <= n; i += 2) {
            result[idx++] = i;
        }
        for (int i = 1; i <= n; i += 2) {
            result[idx++] = i;
        }
    }
    
    for (int i = 0; i < n; i++) {
        std::cout << result[i];
        if (i < n - 1) {
            std::cout << " ";
        }
    }
    std::cout << '\n';
    
    return 0;
}