#include <iostream>
#include <vector>
#include <string>

struct Fraction {
    int num;
    int den;
    std::string original;
};

bool compareFractions(const Fraction& a, const Fraction& b) {
    return (long long)a.num * b.den <= (long long)b.num * a.den;
}

void merge(std::vector<Fraction>& vector, int left, int mid, int right) {
    int num1 = mid - left + 1;
    int num2 = right - mid;
    
    std::vector<Fraction> left_vect(num1);
    std::vector<Fraction> right_vect(num2);
    
    for (int i = 0; i < num1; i++) {
        left_vect[i] = vector[left + i];
    }
    for (int i = 0; i < num2; i++) {
        right_vect[i] = vector[mid + 1 + i];
    }
    
    int i = 0, j = 0, k = left;
    
    while (i < num1 && j < num2) {
        if (compareFractions(left_vect[i], right_vect[j])) {
            vector[k] = left_vect[i];
            i++;
        } else {
            vector[k] = right_vect[j];
            j++;
        }
        k++;
    }
    
    while (i < num1) {
        vector[k] = left_vect[i];
        i++;
        k++;
    }

    while (j < num2) {
        vector[k] = right_vect[j];
        j++;
        k++;
    }
}

void MERGE_SORT(std::vector<Fraction>& vector, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        MERGE_SORT(vector, left, mid);
        MERGE_SORT(vector, mid + 1, right);
        
        merge(vector, left, mid, right);
    }
}

void STABLE_SORT(std::vector<Fraction>& vector) {
    if (vector.empty()) {
        return;
    }
    MERGE_SORT(vector, 0, vector.size() - 1);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    
    std::vector<Fraction> fractions(n);
    
    for (int i = 0; i < n; i++) {
        std::string input;
        std::cin >> input;
        
        size_t slash_pos = input.find('/');
        int num = std::stoi(input.substr(0, slash_pos));
        int den = std::stoi(input.substr(slash_pos + 1));
        
        fractions[i] = {num, den, input};
    }
    
    STABLE_SORT(fractions);
    
    for (int i = 0; i < n; i++) {
        if (i > 0) std::cout << " ";
        std::cout << fractions[i].original;
    }
    std::cout << std::endl;
    
    return 0;
}