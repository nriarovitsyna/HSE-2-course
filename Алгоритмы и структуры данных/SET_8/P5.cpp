#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

size_t levensteinRows(const std::string& str1, const std::string& str2) {
    size_t M = str1.length();
    size_t N = str2.length();

    std::vector<size_t> prevRow(N + 1, 0);
    std::vector<size_t> currRow(N + 1, 0);
    
    for (size_t j = 0; j <= N; j++) {
        prevRow[j] = j;
    }
    
    for (size_t i = 1; i <= M; i++) {
        currRow[0] = i;
        for (size_t j = 1; j <= N; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                currRow[j] = prevRow[j - 1];
            } else {
                currRow[j] = 1 + std::min({
                    currRow[j - 1],
                    prevRow[j],
                    prevRow[j - 1]
                });
            }
        }
        prevRow.swap(currRow);
    }
    return prevRow[N];
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    size_t n;
    std::cin >> n;
    std::cin.ignore();
    
    std::vector<size_t> results;
    results.reserve(n);
    
    for (size_t i = 0; i < n; i++) {
        std::string str1, str2;
        std::getline(std::cin, str1);
        std::getline(std::cin, str2);
        results.push_back(levensteinRows(str1, str2));
    }
    
    for (size_t i = 0; i < n; i++) {
        std::cout << results[i];
        if (i != n - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
    
    return 0;
}