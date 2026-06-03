#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> COUNTING_SORT(const std::vector<int>& vector) {
    if (vector.empty()) {
        return {};
    }
    
    int min = *std::min_element(vector.begin(), vector.end());
    int max = *std::max_element(vector.begin(), vector.end());
    
    int range = max - min + 1;
    std::vector<int> count(range, 0);

    for (int num : vector) {
        count[num - min]++;
    }
    
    std::vector<int> sort_vector;
    sort_vector.reserve(vector.size());
    
    for (int i = 0; i < range; i++) {
        for (int j = 0; j < count[i]; j++) {
            sort_vector.push_back(i + min);
        }
    }
    
    return sort_vector;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> vector(n);
    for (int i = 0; i < n; i++) {
        std::cin >> vector[i];
    }

    std::vector<int> result = COUNTING_SORT(vector);

    for (int i = 0; i < n; i++) {
        std::cout << result[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}