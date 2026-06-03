#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <cstdlib> 
#include <fstream>
#include <vector>
#include <algorithm>

struct Circle {
    double x, y, r;
    
    Circle(double x, double y, double r) : x(x), y(y), r(r) {}
    
    bool isContains(double px, double py) const {
        double dx = px - x;
        double dy = py - y;
        return dx * dx + dy * dy <= r * r;
    }
};

double MONTE_CARLO(const Circle& c1, const Circle& c2, const Circle& c3, 
                     double min_x, double max_x, double min_y, double max_y, 
                     int num_points) {
                        
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    int cnt = 0;
    
    for (int i = 0; i < num_points; ++i) {
        double rnd_x = min_x + (max_x - min_x) * dist(gen);
        double rnd_y = min_y + (max_y - min_y) * dist(gen);
        
        if (c1.isContains(rnd_x, rnd_y) && c2.isContains(rnd_x, rnd_y) && c3.isContains(rnd_x, rnd_y)) {
            cnt++;
        }
    }
    
    double bounding_area = (max_x - min_x) * (max_y - min_y);
    return (static_cast<double>(cnt) / num_points) * bounding_area;
}

double exactArea() {
    const double pi = 3.14159265358979323846;
    return 0.25 * pi + 1.25 * std::asin(0.8) - 1;
}

void runTest() {
    Circle c1(1.0, 1.0, 1.0);
    Circle c2(1.5, 2.0, std::sqrt(5)/2);
    Circle c3(2.0, 1.5, std::sqrt(5)/2);

    double wide_min_x = 0.0;
    double wide_max_x = 2.0 + std::sqrt(5)/2;
    double wide_min_y = 0.0;
    double wide_max_y = 2.0 + std::sqrt(5)/2;
    
    double narrow_min_x = 0.85;
    double narrow_max_x = 2.1;
    double narrow_min_y = 0.85;
    double narrow_max_y = 2.1;
    
    double exact_area = exactArea();
    
    std::ofstream data_file("A1_experiment.csv");
    data_file << "N,Wide_Area,Narrow_Area,Wide_Error,Narrow_Error\n";
    
    for (int N = 100; N <= 100000; N += 500) {
        double wide_area = MONTE_CARLO(c1, c2, c3, wide_min_x, wide_max_x, wide_min_y, wide_max_y, N);
        double narrow_area = MONTE_CARLO(c1, c2, c3, narrow_min_x, narrow_max_x, narrow_min_y, narrow_max_y, N);
        
        double wide_error = std::abs(wide_area - exact_area) / exact_area;
        double narrow_error = std::abs(narrow_area - exact_area) / exact_area;
        
        data_file << N << "," << wide_area << "," << narrow_area << "," 
                  << wide_error << "," << narrow_error << "\n";
    }
    
    data_file.close();
}

int main() {
    runTest();
    return 0;
}