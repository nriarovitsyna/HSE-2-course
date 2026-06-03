#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

std::string trimZeros(const std::string& str) {
    size_t valid_start = str.find_first_not_of('0');
    return (valid_start == std::string::npos) ? "0" : str.substr(valid_start);
}

std::string add(const std::string& num1, const std::string& num2) {
    std::string result;
    int carry = 0;
    int idx1 = num1.length() - 1;
    int idx2 = num2.length() - 1;
    
    while (idx1 >= 0 && idx2 >= 0) {
        int digit_sum = (num1[idx1--] - '0') + (num2[idx2--] - '0') + carry;
        result.push_back((digit_sum % 10) + '0');
        carry = digit_sum / 10;
    }
    
    while (idx1 >= 0) {
        int digit_sum = (num1[idx1--] - '0') + carry;
        result.push_back((digit_sum % 10) + '0');
        carry = digit_sum / 10;
    }
    
    while (idx2 >= 0) {
        int digit_sum = (num2[idx2--] - '0') + carry;
        result.push_back((digit_sum % 10) + '0');
        carry = digit_sum / 10;
    }
    
    if (carry > 0) {
        result.push_back(carry + '0');
    }
    
    std::reverse(result.begin(), result.end());
    return result;
}

std::string subtract(const std::string& num1, const std::string& num2) {
    std::string result;
    int bor = 0;
    int idx1 = num1.length() - 1;
    int idx2 = num2.length() - 1;
    
    while (idx1 >= 0 && idx2 >= 0) {
        int current_digit1 = num1[idx1--] - '0' - bor;
        int current_digit2 = num2[idx2--] - '0';
        
        if (current_digit1 < current_digit2) {
            current_digit1 += 10;
            bor = 1;
        } else {
            bor = 0;
        }
        result.push_back((current_digit1 - current_digit2) + '0');
    }
    
    while (idx1 >= 0) {
        int current_digit1 = num1[idx1--] - '0' - bor;
        
        if (current_digit1 < 0) {
            current_digit1 += 10;
            bor = 1;
        } else {
            bor = 0;
        }
        result.push_back(current_digit1 + '0');
    }
    
    std::reverse(result.begin(), result.end());
    return trimZeros(result);
}

std::string multiply(const std::string& num1, const std::string& num2) {
    if (num1 == "0" || num2 == "0") {
        return "0";
    }
    
    int length1 = num1.length();
    int length2 = num2.length();
    std::vector<int> inter_result(length1 + length2, 0);
    
    for (int i = length1 - 1; i >= 0; i--) {
        for (int j = length2 - 1; j >= 0; j--) {
            int product = (num1[i] - '0') * (num2[j] - '0');
            int sum = product + inter_result[i + j + 1];
            inter_result[i + j + 1] = sum % 10;
            inter_result[i + j] += sum / 10;
        }
    }
    
    std::string result;
    for (int num : inter_result) {
        if (!(result.empty() && num == 0)) {
            result.push_back(num + '0');
        }
    }
    
    return result.empty() ? "0" : result;
}

std::string algorithmKaratsuba(const std::string& num1, const std::string& num2) {
    std::string num1_trim = trimZeros(num1);
    std::string num2_trim = trimZeros(num2);
    
    if (num1_trim == "0" || num2_trim == "0") {
        return "0";
    }
    if (num1_trim.length() == 1 && num2_trim.length() == 1) {
        int product = (num1_trim[0] - '0') * (num2_trim[0] - '0');
        return std::to_string(product);
    }
    if (num1_trim.length() <= 100 || num2_trim.length() <= 100) {
        return multiply(num1_trim, num2_trim);
    }
    
    int max_length = std::max(num1_trim.length(), num2_trim.length());
    int center = (max_length + 1) / 2;
    
    std::string num1_full = std::string(max_length - num1_trim.length(), '0') + num1_trim;
    std::string num2_full = std::string(max_length - num2_trim.length(), '0') + num2_trim;
    
    std::string upper1 = num1_full.substr(0, center);
    std::string lower1 = num1_full.substr(center);
    std::string upper2 = num2_full.substr(0, center);
    std::string lower2 = num2_full.substr(center);
    
    std::string low_product = algorithmKaratsuba(lower1, lower2);
    std::string high_product = algorithmKaratsuba(upper1, upper2);

    std::string sum1 = add(upper1, lower1);
    std::string sum2 = add(upper2, lower2);

    std::string cross_product = subtract(subtract(algorithmKaratsuba(sum1, sum2), high_product), low_product);
    
    std::string high_part = high_product + std::string(2 * (max_length - center), '0');
    std::string mid_part = cross_product + std::string(max_length - center, '0');
    std::string result = add(high_part, mid_part);
    result = add(result, low_product);
    
    return trimZeros(result);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string num1;
    std::string num2;
    std::cin >> num1 >> num2;
    
    std::cout << algorithmKaratsuba(num1, num2) << '\n';
    
    return 0;
}