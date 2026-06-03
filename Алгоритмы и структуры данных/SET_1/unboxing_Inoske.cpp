#include <iostream>
#include <string>
#include <cctype>

class StringDecoder {
private:
    std::string unboxing_str(const std::string& data, size_t& pos) {
        std::string result;
        
        while (pos < data.length() && data[pos] != ']') {
            if (std::isdigit(data[pos])) {
                int mult = 0;

                while (pos < data.length() && std::isdigit(data[pos])) {
                    mult = mult * 10 + (data[pos] - '0');
                    pos++;
                }
                
                if (pos < data.length() && data[pos] == '[') {
                    pos++;
                    std::string cur_str = unboxing_str(data, pos);

                    for (int i = 0; i < mult; i++) {
                        result += cur_str;
                    }
                    
                    if (pos < data.length() && data[pos] == ']') {
                        pos++;
                    }
                }
            } else {
                result += data[pos];
                pos++;
            }
        }
        
        return result;
    }

public:
    std::string decode(const std::string& data) {
        size_t start_pos = 0;
        return unboxing_str(data, start_pos);
    }
};

int main() {
    std::string data;
    std::getline(std::cin, data);
    
    StringDecoder decoder;
    std::cout << decoder.decode(data);
    
    return 0;
}