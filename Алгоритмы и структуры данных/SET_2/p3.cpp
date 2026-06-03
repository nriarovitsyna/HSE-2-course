#include <iostream>
#include <vector>
#include <cstdint>

std::vector<std::vector<int64_t>> add(const std::vector<std::vector<int64_t>>& A, const std::vector<std::vector<int64_t>>& B) {
    int n = A.size();
    std::vector<std::vector<int64_t>> C(n, std::vector<int64_t>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

std::vector<std::vector<int64_t>> subtract(const std::vector<std::vector<int64_t>>& A, const std::vector<std::vector<int64_t>>& B) {
    int n = A.size();
    std::vector<std::vector<int64_t>> C(n, std::vector<int64_t>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

std::vector<std::vector<int64_t>> multiply(const std::vector<std::vector<int64_t>>& A, const std::vector<std::vector<int64_t>>& B) {
    int n = A.size();
    std::vector<std::vector<int64_t>> C(n, std::vector<int64_t>(n, 0));
    
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            if (A[i][k] != 0) {
                for (int j = 0; j < n; j++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    return C;
}

std::vector<std::vector<int64_t>> algorithmStrassen(const std::vector<std::vector<int64_t>>& A, 
                                                    const std::vector<std::vector<int64_t>>& B,
                                                    int limit) {
    int n = A.size();
    
    if (n <= limit) {
        return multiply(A, B);
    }
    
    int half = n / 2;
    
    std::vector<std::vector<int64_t>> a11(half, std::vector<int64_t>(half));
    std::vector<std::vector<int64_t>> a12(half, std::vector<int64_t>(half));
    std::vector<std::vector<int64_t>> a21(half, std::vector<int64_t>(half));
    std::vector<std::vector<int64_t>> a22(half, std::vector<int64_t>(half));
    
    std::vector<std::vector<int64_t>> b11(half, std::vector<int64_t>(half));
    std::vector<std::vector<int64_t>> b12(half, std::vector<int64_t>(half));
    std::vector<std::vector<int64_t>> b21(half, std::vector<int64_t>(half));
    std::vector<std::vector<int64_t>> b22(half, std::vector<int64_t>(half));
    
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            a11[i][j] = A[i][j];
            a12[i][j] = A[i][j + half];
            a21[i][j] = A[i + half][j];
            a22[i][j] = A[i + half][j + half];
            
            b11[i][j] = B[i][j];
            b12[i][j] = B[i][j + half];
            b21[i][j] = B[i + half][j];
            b22[i][j] = B[i + half][j + half];
        }
    }
    
    std::vector<std::vector<int64_t>> Inter1 = algorithmStrassen(add(a11, a22), add(b11, b22), limit);
    std::vector<std::vector<int64_t>> Inter2 = algorithmStrassen(add(a21, a22), b11, limit);
    std::vector<std::vector<int64_t>> Inter3 = algorithmStrassen(a11, subtract(b12, b22), limit);
    std::vector<std::vector<int64_t>> Inter4 = algorithmStrassen(a22, subtract(b21, b11), limit);
    std::vector<std::vector<int64_t>> Inter5 = algorithmStrassen(add(a11, a12), b22, limit);
    std::vector<std::vector<int64_t>> Inter6 = algorithmStrassen(subtract(a21, a11), add(b11, b12), limit);
    std::vector<std::vector<int64_t>> Inter7 = algorithmStrassen(subtract(a12, a22), add(b21, b22), limit);
    
    std::vector<std::vector<int64_t>> c11 = add(subtract(add(Inter1, Inter4), Inter5), Inter7);
    std::vector<std::vector<int64_t>> c12 = add(Inter3, Inter5);
    std::vector<std::vector<int64_t>> c21 = add(Inter2, Inter4);
    std::vector<std::vector<int64_t>> c22 = add(subtract(add(Inter1, Inter3), Inter2), Inter6);
    
    std::vector<std::vector<int64_t>> C(n, std::vector<int64_t>(n));
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            C[i][j] = c11[i][j];
            C[i][j + half] = c12[i][j];
            C[i + half][j] = c21[i][j];
            C[i + half][j + half] = c22[i][j];
        }
    }
    
    return C;
}

std::vector<std::vector<int64_t>> read(int n) {
    std::vector<std::vector<int64_t>> matrix(n, std::vector<int64_t>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> matrix[i][j];
        }
    }
    return matrix;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    
    std::vector<std::vector<int64_t>> A = read(n);
    std::vector<std::vector<int64_t>> B = read(n);
    
    int limit = 64;
    std::vector<std::vector<int64_t>> C = algorithmStrassen(A, B, limit);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << C[i][j];
            if (j < n - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    
    return 0;
}