#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <random>

std::vector<std::vector<bool>> parse(int n, const std::string& str) {
    int row_length = (n + 3) / 4;
    std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n, false));
    
    int idx = 0;
    for (int row_idx = 0; row_idx < n; ++row_idx) {
        std::string row;
        while (idx < str.size() && row.size() < row_length) {
            if (str[idx] != ' ') {
                row += str[idx];
            }
            ++idx;
        }
        
        for (int i = 0; i < row.size(); ++i) {
            char hex_digit = row[i];
            int value;

            if (hex_digit >= '0' && hex_digit <= '9') {
                value = hex_digit - '0';
            }
            else if (hex_digit >= 'A' && hex_digit <= 'F') {
                value = 10 + (hex_digit - 'A');
            }
            else if (hex_digit >= 'a' && hex_digit <= 'f') {
                value = 10 + (hex_digit - 'a');
            }
            else {
                continue;
            }
            
            for (int j = 0; j < 4; ++j) {
                int col_idx = i * 4 + j;
                if (col_idx < n) {
                    matrix[row_idx][col_idx] = (value >> (3 - j)) & 1;
                }
            }
        }
    }
    
    return matrix;
}

std::vector<bool> multiply(const std::vector<std::vector<bool>>& matrix, const std::vector<bool>& vector) {
    int n = matrix.size();
    std::vector<bool> result(n, false);
    
    for (int i = 0; i < n; ++i) {
        bool sum = false;
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] && vector[j]) {
                sum = !sum;
            }
        }
        result[i] = sum;
    }

    return result;
}

bool is_equal(const std::vector<bool>& a, const std::vector<bool>& b) {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

bool FREIVALDS(const std::vector<std::vector<bool>>& A,
                        const std::vector<std::vector<bool>>& B,
                        const std::vector<std::vector<bool>>& C) {
    int n = A.size();
    int tests_cnt = 10;
    std::mt19937 random(100);
    std::uniform_int_distribution<int> bin(0, 1);
    
    for (int t = 0; t < tests_cnt; ++t) {
        std::vector<bool> x(n);
        for (int i = 0; i < n; ++i) {
            x[i] = bin(random);
        }
        
        std::vector<bool> Bx = multiply(B, x);
        std::vector<bool> ABx = multiply(A, Bx);
        std::vector<bool> Cx = multiply(C, x);
        
        if (!is_equal(ABx, Cx)) {
            return false;
        }
    }
    
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    std::cin.ignore();
    
    std::string A_str, B_str, C_str;
    std::getline(std::cin, A_str);
    std::getline(std::cin, B_str);
    std::getline(std::cin, C_str);
    
    auto A = parse(n, A_str);
    auto B = parse(n, B_str);
    auto C = parse(n, C_str);
    
    if (FREIVALDS(A, B, C)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
    
    return 0;
}