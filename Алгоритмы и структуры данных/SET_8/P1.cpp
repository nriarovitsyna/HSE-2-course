#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> computePrefixFun(const std::string& S) {
    size_t s = S.length();
    std::vector<size_t> prefixFun(s + 1, 0);
    
    size_t borderLen = 0;
    prefixFun[1] = 0;

    for (size_t i = 2; i <= s; i++) {
        while (borderLen >= 1 && S[borderLen] != S[i - 1]) {
            borderLen = prefixFun[borderLen];
        }

        if (S[borderLen] == S[i - 1]) {
            ++borderLen;
        } else {
            borderLen = 0;
        }

        prefixFun[i] = borderLen;
    }
    
    return prefixFun;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string S;
    std::cin >> S;
    
    std::vector<size_t> pi = computePrefixFun(S);
    
    for (size_t i = 1; i <= S.length(); i++) {
        std::cout << pi[i];
        if (i != S.length()) {
            std::cout << " ";
        }
    }
    std::cout << '\n';
    
    return 0;
}