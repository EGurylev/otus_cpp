#include <gtest/gtest.h>
#include "ip_adress_pool.h"

TEST(IpFilterTest, InputConversion) {
  std::istringstream is("43.48.1.9\n44.22.33.55\n");
  IPAdressPool ip_pool;
  ip_pool.read(is);
  auto pool = ip_pool.sort_reverse();
  auto ref_pool = std::vector<IPAdress>{{44, 22, 33, 55}, {43, 48, 1, 9}};

  EXPECT_EQ(pool, ref_pool);
}
