#include "StandardSorting.h"
#include "StringSortTester.h"

static void quickSort(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;
    
    std::string pivot = arr[left];
    int i = left + 1;
    int low = left;
    int high = right;
    
    while (i <= high) {
        if (StringSortTester::lessThan(arr[i], pivot)) {
            std::swap(arr[low], arr[i]);
            low++;
            i++;
        } else if (StringSortTester::lessThan(pivot, arr[i])) {
            std::swap(arr[high], arr[i]);
            high--;
        } else {
            i++;
        }
    }
    
    quickSort(arr, left, low - 1);
    quickSort(arr, high + 1, right);
}

void standardQuickSort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    quickSort(arr, 0, arr.size() - 1);
}

static void merge(std::vector<std::string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<std::string> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (StringSortTester::lessThan(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

static void mergeSort(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void standardMergeSort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    mergeSort(arr, 0, arr.size() - 1);
}