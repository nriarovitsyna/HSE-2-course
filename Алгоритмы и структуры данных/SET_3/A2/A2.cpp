#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <string>

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

void MERGE_SORT(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        MERGE_SORT(arr, left, mid);
        MERGE_SORT(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
 }

void MERGE_INSERTION(std::vector<int>& vector, int left, int right, int switch_param) {
    if (left < right) {
        if (right - left + 1 <= switch_param) {
            INSERTION_SORT(vector, left, right);
        } else {
            int mid = left + (right - left) / 2;
            MERGE_INSERTION(vector, left, mid, switch_param);
            MERGE_INSERTION(vector, mid + 1, right, switch_param);
            
            merge(vector, left, mid, right);
        }
    }
}

class ArrayGenerator {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

public:
    ArrayGenerator() : gen(std::random_device{}()), dist(0, 10000) {}

    std::vector<int> generateRandomArray(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = dist(gen);
        }
        return arr;
    }

    std::vector<int> generateReverseSortedArray(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = size - i;
        }
        return arr;
    }

    std::vector<int> generateAlmostSortedArray(int size) {
        int swap_cnt = size / 1000;
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }
        
        for (int i = 0; i < swap_cnt; i++) {
            int idx1 = std::uniform_int_distribution<int>(0, size - 1)(gen);
            int idx2 = std::uniform_int_distribution<int>(0, size - 1)(gen);
            std::swap(arr[idx1], arr[idx2]);
        }
        
        return arr;
    }

    std::vector<int> getSubarray(const std::vector<int>& arr, int size) {
        if (size > arr.size()) {
            return arr;
        }
        return std::vector<int>(arr.begin(), arr.begin() + size);
    }
};

class SortTester {
private:
    ArrayGenerator generator;
    int num;
    int switch_param;

public:
    SortTester(int switch_param, int num = 5) : num(num), switch_param(switch_param) {}

    long long testMerge(const std::vector<int>& original) {
        std::vector<int> arr = original;
        auto start = std::chrono::high_resolution_clock::now();
        
        MERGE_SORT(arr, 0, arr.size() - 1);
        
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    long long testMergeInsertion(const std::vector<int>& original) {
        std::vector<int> arr = original;
        auto start = std::chrono::high_resolution_clock::now();
        
        MERGE_INSERTION(arr, 0, arr.size() - 1, switch_param);
        
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    long long getMeanRuntime(std::vector<int> arr, bool MergeInsertion = true) {
        long long total_time = 0;
        
        for (int i = 0; i < num; i++) {
            std::vector<int> test_arr = arr;
            
            if (MergeInsertion) {
                total_time += testMergeInsertion(test_arr);
            } else {
                total_time += testMerge(test_arr);
            }
        }
        
        return total_time / num;
    }

    bool isSorted(const std::vector<int>& arr) {
        for (size_t i = 1; i < arr.size(); i++) {
            if (arr[i] < arr[i - 1]) {
                return false;
            }
        }
        return true;
    }
};

void runExperiment(int switch_param) {
    ArrayGenerator generator;
    SortTester tester(switch_param, 3);
    
    std::vector<int> randomBase = generator.generateRandomArray(100000);
    std::vector<int> reverseBase = generator.generateReverseSortedArray(100000);
    std::vector<int> almostBase = generator.generateAlmostSortedArray(100000);
    
    std::vector<int> sizes;
    for (int size = 500; size <= 100000; size += 100) {
        sizes.push_back(size);
    }
    
    std::string randomFilename = "random_results(" + std::to_string(switch_param) + ").csv";
    std::string reverseFilename = "reverse_results(" + std::to_string(switch_param) + ").csv";
    std::string almostFilename = "almost_results(" + std::to_string(switch_param) + ").csv";
    
    std::ofstream randomFile(randomFilename);
    std::ofstream reverseFile(reverseFilename);
    std::ofstream almostFile(almostFilename);
    
    randomFile << "Size,Merge,MergeInsertion\n";
    reverseFile << "Size,Merge,MergeInsertion\n";
    almostFile << "Size,Merge,MergeInsertion\n";
    
    for (int size : sizes) {
        std::vector<int> randomArray = generator.getSubarray(randomBase, size);
        long long randomStandard = tester.getMeanRuntime(randomArray, false);
        long long randomHybrid = tester.getMeanRuntime(randomArray, true);
        randomFile << size << "," << randomStandard << "," << randomHybrid << "\n";
        
        std::vector<int> reverseArray = generator.getSubarray(reverseBase, size);
        long long reverseStandard = tester.getMeanRuntime(reverseArray, false);
        long long reverseHybrid = tester.getMeanRuntime(reverseArray, true);
        reverseFile << size << "," << reverseStandard << "," << reverseHybrid << "\n";
        
        std::vector<int> almostArray = generator.getSubarray(almostBase, size);
        long long almostStandard = tester.getMeanRuntime(almostArray, false);
        long long almostHybrid = tester.getMeanRuntime(almostArray, true);
        almostFile << size << "," << almostStandard << "," << almostHybrid << "\n";
    }
    
    randomFile.close();
    reverseFile.close();
    almostFile.close();
}

int main() {
    std::vector<int> switch_params = {5, 10, 15, 20, 30, 50};
    
    for (int param : switch_params) {
        runExperiment(param);
    }
    
    return 0;
}