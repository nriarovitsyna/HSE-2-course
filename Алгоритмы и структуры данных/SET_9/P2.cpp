#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string s;
    std::getline(std::cin, s);
    
    std::unordered_map<std::string, int> dict;
    
    for (int i = 0; i < 128; ++i) {
        std::string ch(1, static_cast<char>(i));
        dict[ch] = i;
    }
    
    int nextCode = 128;
    std::vector<int> result;
    
    std::string current = "";
    for (char c : s) {
        std::string next = current + c;
        
        if (dict.find(next) != dict.end()) {
            current = next;
        } else {
            result.push_back(dict[current]);
            dict[next] = nextCode++;
            current = std::string(1, c);
        }
    }
    
    if (!current.empty()) {
        result.push_back(dict[current]);
    }
    
    std::cout << result.size() << "\n";
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i + 1 < result.size()) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
    
    return 0;
}