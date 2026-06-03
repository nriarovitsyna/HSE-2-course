#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> computePrefixFun(const std::string& S) {
    size_t s = S.length();
    std::vector<size_t> prefixFun(s + 1, 0);
    
    size_t borderLen = 0;

    for (size_t i = 2; i <= s; i++) {
        while (borderLen >= 1 && S[borderLen] != S[i - 1]) {
            borderLen = prefixFun[borderLen];
        }

        if (S[borderLen] == S[i - 1]) {
            ++borderLen;
        }

        prefixFun[i] = borderLen;
    }

    return prefixFun;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string p;
    std::cin >> p;
    
    size_t s = p.length();
    std::vector<size_t> prefixFun = computePrefixFun(p);
    
    if (s % (s - prefixFun[s]) == 0) {
        std::cout << s - prefixFun[s] << '\n';
    } else {
        std::cout << s - prefixFun[s] << '\n';
    }
    
    return 0;
}