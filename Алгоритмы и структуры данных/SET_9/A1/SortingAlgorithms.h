#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <vector>
#include <string>

inline long long stringQuickSortComparisons = 0;
inline long long stringMergeSortComparisons = 0;
inline long long msdRadixSortComparisons = 0;
inline long long msdRadixSortQuickSortComparisons = 0;

void stringMergeSort(std::vector<std::string>& arr);
void stringQuickSort(std::vector<std::string>& arr);
void MSDRadixSort(std::vector<std::string>& arr, int left, int right, int depth, std::vector<std::string>& temp);
void MSDRadixSort_QuickSort(std::vector<std::string>& arr);

#endif