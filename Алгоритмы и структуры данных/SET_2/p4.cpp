#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

class IntervalCalc {
private:
    int _L;
    int _N;
    
    std::vector<int> _T;
    std::vector<int> _V;

    int _totalTime;

    std::vector<int> _sumTime;
    std::vector<int> _sumDist;

public:
    IntervalCalc(int L, int N) : _L(L), _N(N) {
        _T.resize(N);
        _V.resize(N);
        _totalTime = 0;

        for (int i = 0; i < N; i++) {
            std::cin >> _T[i] >> _V[i];
            _totalTime += _T[i];
        }
        
        _sumTime.resize(N + 1, 0);
        _sumDist.resize(N + 1, 0);

        for (int i = 0; i < N; i++) {
            _sumTime[i + 1] = _sumTime[i] + _T[i];
            _sumDist[i + 1] = _sumDist[i] + _T[i] * _V[i];
        }
    }

    double position(double time) const {
        if (time <= 0) {
            return 0.0;
        }
        if (time >= _totalTime) {
            return _sumDist[_N];
        }
        
        int start = 0;
        int end = _N;

        while (start < end) {
            int mid = (start + end) / 2;
            if (time >= _sumTime[mid + 1]) {
                start = mid + 1;
            } else {
                end = mid;
            }
        }
        int i = start;
        return _sumDist[i] + (time - _sumTime[i]) * _V[i];
    }
    
    bool is_valid(double inter) const {
        std::vector<double> points;
        points.push_back(0.0);
        points.push_back(_totalTime - inter);
        
        for (int i = 0; i <= _N; i++) {
            double start = _sumTime[i];
            if (0 <= start && start <= _totalTime - inter) {
                points.push_back(start);
            }

            double shift = _sumTime[i] - inter;
            if (0 <= shift && shift <= _totalTime - inter) {
                points.push_back(shift);
            }
        }
        
        std::sort(points.begin(), points.end());
        points.erase(std::unique(points.begin(), points.end()), points.end());
        
        double min_dist = 1e18;
        for (double s : points) {
            if (s < 0 || s > _totalTime - inter) {
                continue;
            }

            double gap = position(s + inter) - position(s);
            if (gap < min_dist) {
                min_dist = gap;
            }
        }
        return min_dist >= _L - 1e-9;
    }
    
    double findMinInterval() const {
        double min_val = 0.0;
        double max_val = _totalTime;
        for (int iter = 0; iter < 60; iter++) {
            double mid = (min_val + max_val) / 2;
            if (is_valid(mid)) {
                max_val = mid;
            } else {
                min_val = mid;
            }
        }
        
        return max_val;
    }
};

int main() {

    int L;
    int N;
    std::cin >> L >> N;
    
    IntervalCalc inter_calc(L, N);
    double result = inter_calc.findMinInterval();
    std::cout << std::fixed << std::setprecision(6) << result << '\n';

    return 0;
}