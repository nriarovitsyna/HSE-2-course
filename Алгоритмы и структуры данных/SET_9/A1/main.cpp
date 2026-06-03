#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "StringGenerator.h"
#include "StringSortTester.h"
#include "SortingAlgorithms.h"
#include "StandardSorting.h"

int main() {
    const int MAX_SIZE = 3000;
    const int MIN_LEN = 10;
    const int MAX_LEN = 200;
    const int STEP = 100;
    const int RUNS = 5;
    
    // Генерация тестовых данных
    auto randomArray = StringGenerator::generateRandom(MAX_SIZE, MIN_LEN, MAX_LEN);
    auto reversedArray = StringGenerator::generateReversed(MAX_SIZE, MIN_LEN, MAX_LEN);
    auto almostSortedArray = StringGenerator::generateAlmostSorted(MAX_SIZE, MIN_LEN, MAX_LEN);
    
    // Открываем файлы
    std::ofstream randomFile("random_results.csv");
    std::ofstream reversedFile("reversed_results.csv");
    std::ofstream almostSortedFile("almost_sorted_results.csv");
    
    // Заголовки
    randomFile << "Algorithm,Size,Time(mcs),Comparisons\n";
    reversedFile << "Algorithm,Size,Time(mcs),Comparisons\n";
    almostSortedFile << "Algorithm,Size,Time(mcs),Comparisons\n";
    
    for (int size = STEP; size <= MAX_SIZE; size += STEP) {
        std::vector<std::string> testArr;
        long long time, comparisons;
        long long totalTime, totalComparisons;
        
        // ==================== RANDOM ====================
        
        // StandardQuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(randomArray.begin(), randomArray.begin() + size);
            time = StringSortTester::measureTime(testArr, standardQuickSort);
            comparisons = StringSortTester::getComparisonCount();
            totalTime += time;
            totalComparisons += comparisons;
        }
        randomFile << "StandardQuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StandardMergeSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(randomArray.begin(), randomArray.begin() + size);
            time = StringSortTester::measureTime(testArr, standardMergeSort);
            comparisons = StringSortTester::getComparisonCount();
            totalTime += time;
            totalComparisons += comparisons;
        }
        randomFile << "StandardMergeSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StringQuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(randomArray.begin(), randomArray.begin() + size);
            time = StringSortTester::measureTime(testArr, stringQuickSort);
            comparisons = stringQuickSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        randomFile << "StringQuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StringMergeSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(randomArray.begin(), randomArray.begin() + size);
            time = StringSortTester::measureTime(testArr, stringMergeSort);
            comparisons = stringMergeSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        randomFile << "StringMergeSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // MSDRadixSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(randomArray.begin(), randomArray.begin() + size);
            std::vector<std::string> temp(size);
            auto sortFunc = [&temp](std::vector<std::string>& a) {
                MSDRadixSort(a, 0, a.size() - 1, 0, temp);
            };
            time = StringSortTester::measureTime(testArr, sortFunc);
            comparisons = msdRadixSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        randomFile << "MSDRadixSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // MSDRadixSort_QuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(randomArray.begin(), randomArray.begin() + size);
            time = StringSortTester::measureTime(testArr, MSDRadixSort_QuickSort);
            comparisons = msdRadixSortQuickSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        randomFile << "MSDRadixSort_QuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // ==================== REVERSED ====================
        
        // StandardQuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(reversedArray.begin(), reversedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, standardQuickSort);
            comparisons = StringSortTester::getComparisonCount();
            totalTime += time;
            totalComparisons += comparisons;
        }
        reversedFile << "StandardQuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StandardMergeSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(reversedArray.begin(), reversedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, standardMergeSort);
            comparisons = StringSortTester::getComparisonCount();
            totalTime += time;
            totalComparisons += comparisons;
        }
        reversedFile << "StandardMergeSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StringQuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(reversedArray.begin(), reversedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, stringQuickSort);
            comparisons = stringQuickSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        reversedFile << "StringQuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StringMergeSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(reversedArray.begin(), reversedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, stringMergeSort);
            comparisons = stringMergeSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        reversedFile << "StringMergeSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // MSDRadixSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(reversedArray.begin(), reversedArray.begin() + size);
            std::vector<std::string> temp(size);
            auto sortFunc = [&temp](std::vector<std::string>& a) {
                MSDRadixSort(a, 0, a.size() - 1, 0, temp);
            };
            time = StringSortTester::measureTime(testArr, sortFunc);
            comparisons = msdRadixSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        reversedFile << "MSDRadixSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // MSDRadixSort_QuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(reversedArray.begin(), reversedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, MSDRadixSort_QuickSort);
            comparisons = msdRadixSortQuickSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        reversedFile << "MSDRadixSort_QuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // ==================== ALMOST SORTED ====================
        
        // StandardQuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(almostSortedArray.begin(), almostSortedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, standardQuickSort);
            comparisons = StringSortTester::getComparisonCount();
            totalTime += time;
            totalComparisons += comparisons;
        }
        almostSortedFile << "StandardQuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StandardMergeSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(almostSortedArray.begin(), almostSortedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, standardMergeSort);
            comparisons = StringSortTester::getComparisonCount();
            totalTime += time;
            totalComparisons += comparisons;
        }
        almostSortedFile << "StandardMergeSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StringQuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(almostSortedArray.begin(), almostSortedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, stringQuickSort);
            comparisons = stringQuickSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        almostSortedFile << "StringQuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // StringMergeSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(almostSortedArray.begin(), almostSortedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, stringMergeSort);
            comparisons = stringMergeSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        almostSortedFile << "StringMergeSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // MSDRadixSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(almostSortedArray.begin(), almostSortedArray.begin() + size);
            std::vector<std::string> temp(size);
            auto sortFunc = [&temp](std::vector<std::string>& a) {
                MSDRadixSort(a, 0, a.size() - 1, 0, temp);
            };
            time = StringSortTester::measureTime(testArr, sortFunc);
            comparisons = msdRadixSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        almostSortedFile << "MSDRadixSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        // MSDRadixSort_QuickSort
        totalTime = 0; totalComparisons = 0;
        for (int r = 0; r < RUNS; r++) {
            testArr.assign(almostSortedArray.begin(), almostSortedArray.begin() + size);
            time = StringSortTester::measureTime(testArr, MSDRadixSort_QuickSort);
            comparisons = msdRadixSortQuickSortComparisons;
            totalTime += time;
            totalComparisons += comparisons;
        }
        almostSortedFile << "MSDRadixSort_QuickSort," << size << "," << totalTime/RUNS << "," << totalComparisons/RUNS << "\n";
        
        std::cout << "Размер " << size << " готов." << "\n";
    }
    
    randomFile.close();
    reversedFile.close();
    almostSortedFile.close();
    
    std::cout << "Готово! Файлы: random_results.csv, reversed_results.csv, almost_sorted_results.csv" << "\n";
    
    return 0;
}