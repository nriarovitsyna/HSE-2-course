#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string P, T;
    std::cin >> P >> T;
    
    size_t M = P.length();
    size_t N = T.length();
    
    if (M == 0 || M > N) {
        std::cout << "0\n";
        return 0;
    }
    
    std::vector<size_t> shift(256, M);
    
    for (size_t j = 0; j < M - 1; j++) {
        shift[static_cast<unsigned char>(P[j])] = M - 1 - j;
    }
    
    std::vector<size_t> positions;
    size_t i = M - 1;
    
    while (i < N) {
        size_t j = M - 1;
        size_t k = i;

        while (j >= 0 && k < N && P[j] == T[k]) {
            if (j == 0) {
                positions.push_back(k);
                break;
            }
            j--;
            k--;
        }
        
        i += shift[static_cast<unsigned char>(T[i])];
    }

    std::cout << positions.size() << "\n";
    for (size_t pos : positions) {
        std::cout << pos << "\n";
    }
    
    return 0;
}