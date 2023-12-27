#include <gtest/gtest.h>
#include "lib.h"

TEST(HelloTest, BasicAssertions) {
  EXPECT_NE(version(), 100);
}
