#include "SortingAlgorithms.h"
#include <algorithm>

static void MSDRadixSortInternal(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp);

static void countingSort(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp) {
    if (left >= right) {
        return;
    }
    
    int count[256] = {0};
    
    for (int i = left; i <= right; i++) {
        unsigned char c;
        if (depth < (int)arr[i].length()) {
            c = (unsigned char)arr[i][depth];
        } else {
            c = 0;
        }
        count[c + 1]++;
    }
    
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = left; i <= right; i++) {
        unsigned char c;
        if (depth < (int)arr[i].length()) {
            c = (unsigned char)arr[i][depth];
        } else {
            c = 0;
        }
        temp[count[c] + left] = arr[i];
        count[c]++;
    }
    
    for (int i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
    
    int start = left;
    for (int i = 0; i <= 255; i++) {
        int end = left + count[i] - 1;
        if (start <= end) {
            if (i > 0) {
                MSDRadixSortInternal(arr, start, end, depth + 1, temp);
            }
            start = end + 1;
        }
    }
}

static void MSDRadixSortInternal(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp) {
    if (left >= right) {
        return;
    }
    
    countingSort(arr, left, right, depth, temp);
}

void MSDRadixSort(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp) {
    msdRadixSortComparisons = 0;
    MSDRadixSortInternal(arr, left, right, depth, temp);
}