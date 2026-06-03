#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string S;
    std::cin >> S;
    
    size_t n = S.length();

    std::string newS(2 * n + 1, '#');
    for (size_t i = 0; i < n; i++) {
        newS[2 * i + 1] = S[i];
    }
    
    size_t newN = newS.length();
    std::vector<size_t> palLen(newN, 0);
    
    size_t leftBorder = 0;
    size_t rightBorder = 0;
    size_t count = 0;
    
    for (size_t i = 0; i < newN; i++) {
        if (i <= rightBorder) {
            palLen[i] = std::min(palLen[leftBorder + (rightBorder - i)], rightBorder - i);
        }
        
        while (i >= palLen[i] && i + palLen[i] < newN && newS[i - palLen[i]] == newS[i + palLen[i]]) {
            palLen[i]++;
        }
        palLen[i]--;
        
        if (i + palLen[i] > rightBorder) {
            leftBorder = i - palLen[i];
            rightBorder = i + palLen[i];
        }
        
        count += (palLen[i] + 1) / 2;
    }
    
    std::cout << count << "\n";
    
    return 0;
}