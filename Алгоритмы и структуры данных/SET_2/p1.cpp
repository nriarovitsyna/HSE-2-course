#include <iostream>
#include <vector>

struct Interval {
    int left;
    int right;
    
    Interval(int start = 0, int end = 0) {
        left = start;
        right = end;
    }
    
    size_t length() const {
        return (right >= left) ? (right - left + 1) : 0;
    }
    
    Interval overlap(const Interval& other) const {
        int lowerBound = (left > other.left) ? left : other.left;
        int upperBound = (right < other.right) ? right : other.right;
        
        if (lowerBound <= upperBound) {
            return Interval(lowerBound, upperBound);
        }
        return Interval(0, -1);
    }
    
    bool isCorrect() const {
        return left <= right;
    }
};

void unification(std::vector<Interval>& intervals, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;
    
    std::vector<Interval> leftInter(leftSize);
    std::vector<Interval> rightInter(rightSize);
    
    for (int leftCnt = 0; leftCnt < leftSize; leftCnt++) {
        leftInter[leftCnt] = intervals[left + leftCnt];
    }
    for (int rightCnt = 0; rightCnt < rightSize; rightCnt++) {
        rightInter[rightCnt] = intervals[mid + 1 + rightCnt];
    }
    
    int leftCnt = 0;
    int rightCnt = 0;
    int resultCnt = left;
    
    while (leftCnt < leftSize && rightCnt < rightSize) {
        if (leftInter[leftCnt].left < rightInter[rightCnt].left) {
            intervals[resultCnt] = leftInter[leftCnt];
            leftCnt++;
        } else if (leftInter[leftCnt].left > rightInter[rightCnt].left) {
            intervals[resultCnt] = rightInter[rightCnt];
            rightCnt++;
        } else {
            if (leftInter[leftCnt].right <= rightInter[rightCnt].right) {
                intervals[resultCnt] = leftInter[leftCnt];
                leftCnt++;
            } else {
                intervals[resultCnt] = rightInter[rightCnt];
                rightCnt++;
            }
        }
        resultCnt++;
    }
    
    while (leftCnt < leftSize) {
        intervals[resultCnt] = leftInter[leftCnt];
        leftCnt++;
        resultCnt++;
    }
    
    while (rightCnt < rightSize) {
        intervals[resultCnt] = rightInter[rightCnt];
        rightCnt++;
        resultCnt++;
    }
}

void sortByUnification(std::vector<Interval>& intervals, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        sortByUnification(intervals, left, mid);
        sortByUnification(intervals, mid + 1, right);
        
        unification(intervals, left, mid, right);
    }
}

bool isBetter(const std::pair<Interval, size_t>& leftPair, const std::pair<Interval, size_t>& rightPair) {
    if (leftPair.second != rightPair.second) {
        return leftPair.second > rightPair.second;
    }

    if (!leftPair.first.isCorrect()) {
        return false;
    }

    if (!rightPair.first.isCorrect()) {
        return true;
    }

    if (leftPair.first.left != rightPair.first.left) {
        return leftPair.first.left < rightPair.first.left;
    }

    return leftPair.first.right < rightPair.first.right;
}

std::pair<Interval, size_t> getMaxCommonInter(std::vector<Interval>& intervals, int left, int right) {
    if (left == right) {
        return std::make_pair(Interval(0, -1), 0);
    }
    
    if (right - left == 1) {
        Interval commonInter = intervals[left].overlap(intervals[right]);
        if (commonInter.isCorrect()) {
            return std::make_pair(commonInter, commonInter.length());
        }
        return std::make_pair(Interval(0, -1), 0);
    }
    
    int mid = left + (right - left) / 2;
    
    std::pair<Interval, size_t> leftMax = getMaxCommonInter(intervals, left, mid);
    std::pair<Interval, size_t> rightMax = getMaxCommonInter(intervals, mid + 1, right);

    Interval maxRight = intervals[left];
    for (int i = left + 1; i <= mid; i++) {
        if (intervals[i].right > maxRight.right) {
            maxRight = intervals[i];
        }
    }
    
    std::pair<Interval, size_t> crossMax = std::make_pair(Interval(0, -1), 0);
    for (int i = mid + 1; i <= right; i++) {
        Interval commonInter = maxRight.overlap(intervals[i]);
        if (commonInter.isCorrect()) {
            std::pair<Interval, size_t> cur = std::make_pair(commonInter, commonInter.length());
            if (isBetter(cur, crossMax)) {
                crossMax = cur;
            }
        }
    }
    
    Interval minLeft = intervals[mid + 1];
    for (int i = mid + 2; i <= right; i++) {
        if (intervals[i].left < minLeft.left) {
            minLeft = intervals[i];
        }
    }
    
    for (int i = left; i <= mid; i++) {
        Interval commonInter = intervals[i].overlap(minLeft);
        if (commonInter.isCorrect()) {
            std::pair<Interval, size_t> cur = std::make_pair(commonInter, commonInter.length());
            if (isBetter(cur, crossMax)) {
                crossMax = cur;
            }
        }
    }

    std::pair<Interval, size_t> bestRes = leftMax;
    if (isBetter(rightMax, bestRes)) bestRes = rightMax;
    if (isBetter(crossMax, bestRes)) bestRes = crossMax;
    
    return bestRes;
}

int main() {
    int n;
    std::cin >> n;
    
    std::vector<Interval> intervals;
    
    for (int i = 0; i < n; i++) {
        int start, end;
        std::cin >> start >> end;
        intervals.push_back(Interval(start, end));
    }
    
    if (n < 2) {
        std::cout << 0 << '\n';
        return 0;
    }
    
    sortByUnification(intervals, 0, n - 1);
    
    std::pair<Interval, size_t> result = getMaxCommonInter(intervals, 0, n - 1);
    
    std::cout << result.second << '\n';
    
    if (result.second > 0) {
        std::cout << result.first.left << " " << result.first.right << '\n';
    }
    
    return 0;
}