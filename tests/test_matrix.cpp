#include <gtest/gtest.h>

#include "matrix.h"

TEST(MatrixTest, EmptyMatrix) {
    Matrix<int, -1> m;

    EXPECT_TRUE(m.size() == 0);
    EXPECT_TRUE(m.empty() == true);

    for (int i{}; i < 100; i += 10) {
        EXPECT_TRUE(m[i][i] == -1);
    }

    EXPECT_TRUE(m.size() == 0);
    EXPECT_TRUE(m.empty() == true);

    for (int i{}; i < 100; i += 10) {
        m[i][i] = -1;
    }

    EXPECT_TRUE(m.size() == 0);
    EXPECT_TRUE(m.empty() == true);
}

TEST(MatrixTest, NonEmptyMatrix) {
    Matrix<int, 1> m;

    EXPECT_TRUE(m.size() == 0);
    EXPECT_TRUE(m.empty() == true);

    for (int i{}; i < 100; i += 10) {
        m[i][i] = i;
    }
    EXPECT_TRUE(m.size() == 10);
    EXPECT_TRUE(m.empty() == false);

    EXPECT_TRUE(m[1][1] == 1);

    m[20][20] = 1;
    m[50][50] = 1;
    EXPECT_TRUE(m.size() == 8);

    m[10][10] = 15;
    EXPECT_TRUE(m[10][10] == 15);
}

TEST(MatrixTest, MatrixIteration) {
    Matrix<int, -1> m;

    EXPECT_TRUE(m.size() == 0);
    EXPECT_TRUE(m.empty() == true);

    for (int i{}; i < 10; ++i) {
        m[i][i] = i;
    }
    EXPECT_TRUE(m.size() == 10);
    EXPECT_TRUE(m.empty() == false);

    for (auto [r, c, v] : m) {
        EXPECT_TRUE(r == c && static_cast<int>(c) == v);
    }
}