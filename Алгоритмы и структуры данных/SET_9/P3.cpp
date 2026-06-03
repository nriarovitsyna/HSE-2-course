#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int k;
    std::cin >> k;
    
    std::vector<int> codes(k);
    for (int i = 0; i < k; ++i) {
        std::cin >> codes[i];
    }
    
    std::vector<std::string> dict(128);
    for (int i = 0; i < 128; ++i) {
        dict[i] = std::string(1, static_cast<char>(i));
    }
    
    std::string result;
    std::string prev = dict[codes[0]];
    result += prev;
    
    for (int i = 1; i < k; ++i) {
        int code = codes[i];
        std::string current;
        
        if (code < static_cast<int>(dict.size())) {
            current = dict[code];
        } else {
            current = prev + prev[0];
        }
        
        result += current;
        dict.push_back(prev + current[0]);
        prev = current;
    }
    
    std::cout << result << "\n";
    
    return 0;
}