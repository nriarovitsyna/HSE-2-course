#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <cstdlib> 

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
                                double min_x, double max_x, double min_y, double max_y) {
    const int num_points = 500000;

    std::srand(std::time(0));
    
    int cnt = 0;
    
    for (int i = 0; i < num_points; ++i) {
        double rnd_x = min_x + (max_x - min_x) * (std::rand() / (double)RAND_MAX);
        double rnd_y = min_y + (max_y - min_y) * (std::rand() / (double)RAND_MAX);
        
        if (c1.isContains(rnd_x, rnd_y) && c2.isContains(rnd_x, rnd_y) && c3.isContains(rnd_x, rnd_y)) {
            cnt++;
        }
    }
    
    double bounding_area = (max_x - min_x) * (max_y - min_y);
    return (static_cast<double>(cnt) / num_points) * bounding_area;
}

int main() {
    double x1, y1, r1, x2, y2, r2, x3, y3, r3;
    std::cin >> x1 >> y1 >> r1;
    std::cin >> x2 >> y2 >> r2;
    std::cin >> x3 >> y3 >> r3;
    
    Circle c1(x1, y1, r1);
    Circle c2(x2, y2, r2);
    Circle c3(x3, y3, r3);
    
    double min_x = std::min(std::min(x1 - r1, x2 - r2), x3 - r3);
    double max_x = std::max(std::max(x1 + r1, x2 + r2), x3 + r3);
    double min_y = std::min(std::min(y1 - r1, y2 - r2), y3 - r3);
    double max_y = std::max(std::max(y1 + r1, y2 + r2), y3 + r3);
    
    double area = MONTE_CARLO(c1, c2, c3, min_x, max_x, min_y, max_y);

    std::cout.precision(20);
    std::cout << std::fixed << area << '\n';
    
    return 0;
}