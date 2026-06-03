#include <iostream>
#include <vector>
#include <algorithm>

unsigned char getByte(unsigned int num, int byte_pos) {
    return (num >> (byte_pos * 8)) & 0xFF;
}

void RADIX_SORT(std::vector<int>& vector) {
    const int n = vector.size();
    if (n <= 1) {
        return;
    }

    std::vector<unsigned int> temp(n);
    for (int i = 0; i < n; ++i) {
        temp[i] = static_cast<unsigned int>(vector[i]) ^ 0x80000000;
    }
    
    const int byte_cnt = 4;
    std::vector<unsigned int> sorted(n);
    
    for (int byte_pos = 0; byte_pos < byte_cnt; ++byte_pos) {
        std::vector<int> counter(256, 0);
        
        for (int i = 0; i < n; ++i) {
            counter[getByte(temp[i], byte_pos)]++;
        }
        
        for (int i = 1; i < 256; ++i) {
            counter[i] += counter[i - 1];
        }
        
        for (int i = n - 1; i >= 0; --i) {
            unsigned char byte_val = getByte(temp[i], byte_pos);
            sorted[counter[byte_val] - 1] = temp[i];
            counter[byte_val]--;
        }
        
        temp = sorted;
    }
    
    for (int i = 0; i < n; ++i) {
        vector[i] = static_cast<int>(temp[i] ^ 0x80000000);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    std::vector<int> vector(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> vector[i];
    }
    
    RADIX_SORT(vector);
    
    for (int i = 0; i < n; ++i) {
        std::cout << vector[i] << " ";
    }
    std::cout << '\n';
    
    return 0;
}