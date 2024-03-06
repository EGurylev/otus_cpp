#include "matrix.h"
#include <iostream>

int main() {
    Matrix<int, 0> m;

    for (int i{}; i < 10; ++i) {
        m[i][i] = i;
        m[i][9 - i] = 9 - i;
    }

    for (int i{1}; i < 9; ++i) {
        for (int j{1}; j < 9; ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << m.size() << std::endl;

    for (auto [x, y, z] : m) {
        std::cout << x << " " << y << " " << z << std::endl;
    }
}