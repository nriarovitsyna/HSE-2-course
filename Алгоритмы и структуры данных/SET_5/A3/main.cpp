#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <random>

#include "RandomStreamGen.h"      
#include "HashFuncGen.h"         
#include "HyperLogLog.h"  

int main() {
    std::vector<int> streamSizes = {5000, 20000, 50000, 100000};
    int perSize = 3;
    int totalStreams = streamSizes.size() * perSize;
    
    std::ofstream outFile("results.csv");
    if (!outFile.is_open()) {
        std::cerr << "Ошибка открытия файла для записи!" << "\n";
        return 1;
    }
    
    outFile << "stream_id,stream_size,percentage,exact,estimate" << "\n";
    
    int streamId = 0;
    for (int size : streamSizes) {
        for (int g = 0; g < perSize; ++g) {
            RandomStreamGen gen;
            gen.generate(size);
            
            for (int p = 10; p <= 100; p += 10) {
                auto part = gen.getPart(p);
                size_t exact = exactUnique(part);
                
                HyperLogLog hll;
                for (const auto& s : part) hll.add(s);
                double estimate = hll.estimate();
                
                outFile << streamId << "," << size << "," << p << "," 
                       << exact << "," << std::fixed << std::setprecision(2) 
                       << estimate << "\n";
            }
            streamId++;
        }
    }
    
    outFile.close();
    std::cout << "Сгенерировано " << totalStreams << " потоков, данные сохранены в results.csv" << "\n";
    
    return 0;
}