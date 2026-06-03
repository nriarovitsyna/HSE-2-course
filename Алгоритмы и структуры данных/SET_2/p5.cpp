#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

struct Point {
    long long x;
    long long y;
    Point(long long x = 0, long long y = 0) : x(x), y(y) {}
};

long long getSquaredDist(const Point& point1, const Point& point2) {
    long long diffX = point1.x - point2.x;
    long long diffY = point1.y - point2.y;
    return diffX * diffX + diffY * diffY;
}

bool compareByX(const Point& point1, const Point& point2) {
    return point1.x < point2.x || (point1.x == point2.x && point1.y < point2.y);
}

bool compareByY(const Point& point1, const Point& point2) {
    return point1.y < point2.y;
}

void findClosest(std::vector<Point>& points, std::vector<Point>& buffer, long long& min_dist, int start, int end) {
    if (end - start <= 3) {
        for (int i = start; i < end; ++i) {
            for (int j = i + 1; j < end; ++j) {
                min_dist = std::min(min_dist, getSquaredDist(points[i], points[j]));
            }
        }
        std::sort(points.begin() + start, points.begin() + end, compareByY);
        return;
    }
    
    int mid = (start + end) / 2;
    long long mid_x = points[mid].x;
    findClosest(points, buffer, min_dist, start, mid);
    findClosest(points, buffer, min_dist, mid, end);
    
    std::merge(points.begin() + start, points.begin() + mid,
              points.begin() + mid, points.begin() + end,
              buffer.begin(), compareByY);
    std::copy(buffer.begin(), buffer.begin() + (end - start), points.begin() + start);
    
    std::vector<Point> center_strip;
    for (int i = start; i < end; ++i) {
        long long diffX = points[i].x - mid_x;
        if (diffX * diffX < min_dist) {
            for (int j = (int)center_strip.size() - 1; j >= 0; --j) {
                long long diffY = points[i].y - center_strip[j].y;
                if (diffY * diffY >= min_dist) break;
                min_dist = std::min(min_dist, getSquaredDist(points[i], center_strip[j]));
            }
            center_strip.push_back(points[i]);
        }
    }
}

long long getMinDistance(std::vector<Point>& points) {
    int size = points.size();
    if (size <= 1) {
        return std::numeric_limits<long long>::max();
    }
    
    std::sort(points.begin(), points.end(), compareByX);
    
    std::vector<Point> buffer(size);
    long long min_dist = std::numeric_limits<long long>::max();
    
    findClosest(points, buffer, min_dist, 0, size);
    return min_dist;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::vector<Point> points;
    long long x;
    long long y;
    while (std::cin >> x >> y) {
        points.emplace_back(x, y);
    }
    
    long long result = getMinDistance(points);
    std::cout << (long long)std::sqrt(result) << '\n';
    
    return 0;
}