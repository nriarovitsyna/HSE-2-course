#include "SortingAlgorithms.h"
#include <algorithm>

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
        
        ++stringQuickSortComparisons;
        if (currentChar < pivotChar) {
            std::swap(arr[lowBorder], arr[i]);
            lowBorder++;
            i++;
        } else {
            ++stringQuickSortComparisons;
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

void stringQuickSort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    stringQuickSortComparisons = 0;
    QuickSort(arr, 0, arr.size() - 1, 0);
}