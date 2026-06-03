#include <iostream>
#include <vector>

void heapify(std::vector<int>& A, int i, int n) {
    int max_idx = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && A[left] > A[max_idx])
        max_idx = left;

    if (right < n && A[right] > A[max_idx])
        max_idx = right;

    if (max_idx != i) {
        std::swap(A[i], A[max_idx]);

        heapify(A, max_idx, n);
    }
}

void buildMaxHeap(std::vector<int>& A, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(A, i, n);
}

void heapSort(std::vector<int>& A, int n) {
    buildMaxHeap(A, n);

    for (int i = n - 1; i > 0; i--) {
        std::swap(A[0], A[i]);

        heapify(A, 0, i);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int n;
    std::cin >> n;
    
    std::vector<int> A(n);
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
    }
    
    heapSort(A, n);
    
    for (int i = 0; i < n; i++) {
        std::cout << A[i];
        if (i < n - 1) std::cout << " ";
    }
    std::cout << '\n';
    
    return 0;
}