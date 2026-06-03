#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> computePrefixFunStrong(const std::string& P) {
    size_t M = P.length();
    std::vector<size_t> prefixFun(M + 1, 0);

    size_t borderLen = 0;

    for (size_t i = 2; i <= M; i++) {
        while (borderLen >= 1 && P[borderLen] != P[i - 1]) {
            borderLen = prefixFun[borderLen];
        }

        if (P[borderLen] == P[i - 1]) {
            ++borderLen;
        }

        prefixFun[i] = borderLen;
    }
    
    std::vector<size_t> prefixFunStrong(M + 1, 0);

    for (size_t i = 1; i <= M; i++) {
        if (i < M && P[prefixFun[i]] == P[i]) {
            prefixFunStrong[i] = prefixFunStrong[prefixFun[i]];
        } else {
            prefixFunStrong[i] = prefixFun[i];
        }
    }
    
    return prefixFunStrong;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string P, T;
    std::cin >> P >> T;
    
    size_t M = P.length();
    size_t N = T.length();
    
    if (M == 0) {
        std::cout << "0\n";
        return 0;
    }
    
    std::vector<size_t> prefixFun = computePrefixFunStrong(P);
    
    std::vector<size_t> positions;
    size_t matched = 0;
    
    for (size_t i = 1; i <= N; i++) {
        while (matched >= 1 && P[matched] != T[i - 1]) {
            matched = prefixFun[matched];
        }

        if (P[matched] == T[i - 1]) {
            ++matched;
        }

        if (matched == M) {
            positions.push_back(i - M);
            matched = prefixFun[matched];
        }
    }

    std::cout << positions.size() << "\n";
    for (size_t pos : positions) {
        std::cout << pos << "\n";
    }
    
    return 0;
}