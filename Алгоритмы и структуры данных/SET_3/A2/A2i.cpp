#include <iostream>
#include <vector>
#include <algorithm>

const int switch_param = 15;

void merge(std::vector<int>& vector, int left, int mid, int right) {
    int left_size = mid - left + 1;
    int right_size = right - mid;
    
    std::vector<int> left_vect(left_size);
    std::vector<int> rightArr(right_size);
    
    for (int i = 0; i < left_size; i++) {
        left_vect[i] = vector[left + i];
    }
    for (int j = 0; j < right_size; j++) {
        rightArr[j] = vector[mid + 1 + j];
    }
    
    int right_idx = 0;
    int left_idx = 0;
    int merege_idx = left;
    
    while (right_idx < left_size && left_idx < right_size) {
        if (left_vect[right_idx] <= rightArr[left_idx]) {
            vector[merege_idx] = left_vect[right_idx];
            right_idx++;
        } else {
            vector[merege_idx] = rightArr[left_idx];
            left_idx++;
        }
        merege_idx++;
    }
    
    while (right_idx < left_size) {
        vector[merege_idx] = left_vect[right_idx];
        right_idx++;
        merege_idx++;
    }
    
    while (left_idx < right_size) {
        vector[merege_idx] = rightArr[left_idx];
        left_idx++;
        merege_idx++;
    }
}

void INSERTION_SORT(std::vector<int>& vector, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = vector[i];
        int j = i - 1;

        while (j >= left && vector[j] > key) {
            vector[j + 1] = vector[j];
            j--;
        }
        vector[j + 1] = key;
    }
}

void MERGE_INSERTION(std::vector<int>& vector, int left, int right) {
    if (left < right) {
        if (right - left + 1 <= switch_param) {
            INSERTION_SORT(vector, left, right);
        } else {
            int mid = left + (right - left) / 2;
            MERGE_INSERTION(vector, left, mid);
            MERGE_INSERTION(vector, mid + 1, right);
            
            merge(vector, left, mid, right);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    
    std::vector<int> vector(n);
    for (int i = 0; i < n; i++) {
        std::cin >> vector[i];
    }
    
    if (n >= 1) {
        MERGE_INSERTION(vector, 0, n - 1);
    }
    
    for (int i = 0; i < n; i++) {
        std::cout << vector[i];
        if (i < n - 1) {
            std::cout << " ";
        }
    }
    std::cout << '\n';
    
    return 0;
}