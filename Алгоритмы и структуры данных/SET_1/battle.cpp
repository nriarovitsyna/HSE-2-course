#include <iostream>
#include <deque>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    std::cin >> T;

    std::deque<int> front_dq;
    std::deque<int> back_dq;

    for (int t = 1; t <= T; ++t) {
        char command;
        std::cin >> command;

        if (command == '*') {
            int num;
            std::cin >> num;
            back_dq.push_front(num);
        } else if (command == '+') {
            int num;
            std::cin >> num;
            back_dq.push_back(num);
        } else if (command == '-') {
            if (front_dq.empty()) {
                front_dq.push_back(back_dq.front());
                back_dq.pop_front();
            }

            std::cout << front_dq.front() << '\n';
            front_dq.pop_front();
        }

        if (!back_dq.empty() && back_dq.size() > front_dq.size()) {
            front_dq.push_back(back_dq.front());
            back_dq.pop_front();
        }
    }

    return 0;
}