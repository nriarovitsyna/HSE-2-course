#include "SortingAlgorithms.h"
#include <algorithm>

const int ALPHABET_SIZE = 74;

static void QuickSort(std::vector<std::string>& arr, int left, int right, int depth) {
    if (left >= right) {
        return;
    }
    
    int lowBorder = left;
    int highBorder = right;
    int i = left + 1;
    
    std::string pivot = arr[left];
    char pivotChar;
    if ((int)pivot.length() > depth) {
        pivotChar = pivot[depth];
    } else {
        pivotChar = 0;
    }
    
    while (i <= highBorder) {
        char currentChar;
        if ((int)arr[i].length() > depth) {
            currentChar = arr[i][depth];
        } else {
            currentChar = 0;
        }
        
        ++msdRadixSortQuickSortComparisons;
        if (currentChar < pivotChar) {
            std::swap(arr[lowBorder], arr[i]);
            lowBorder++;
            i++;
        } else {
            ++msdRadixSortQuickSortComparisons;
            if (currentChar > pivotChar) {
                std::swap(arr[highBorder], arr[i]);
                highBorder--;
            } else {
                i++;
            }
        }
    }
    
    if (pivotChar == 0) {
        QuickSort(arr, left, lowBorder - 1, depth);
        QuickSort(arr, highBorder + 1, right, depth);
    } else {
        QuickSort(arr, left, lowBorder - 1, depth);
        QuickSort(arr, highBorder + 1, right, depth);
        QuickSort(arr, lowBorder, highBorder, depth + 1);
    }
}

static void MSDRadixSort_QuickSort_Internal(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp);

static void countingSort(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp) {
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
                MSDRadixSort_QuickSort_Internal(arr, start, end, depth + 1, temp);
            }
            start = end + 1;
        }
    }
}

static void MSDRadixSort_QuickSort_Internal(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp) {
    if (left >= right) {
        return;
    }
    
    int size = right - left + 1;
    
    if (size < ALPHABET_SIZE) {
        QuickSort(arr, left, right, depth);
    } else {
        countingSort(arr, left, right, depth, temp);
    }
}

void MSDRadixSort_QuickSort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    msdRadixSortQuickSortComparisons = 0;
    std::vector<std::string> temp(arr.size());
    MSDRadixSort_QuickSort_Internal(arr, 0, arr.size() - 1, 0, temp);
}