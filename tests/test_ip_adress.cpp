#include <gtest/gtest.h>
#include "ip_adress_pool.h"

TEST(IpFilterInputConversion, InvalidInputDataTest) {
  std::istringstream is("43.4809.1.9\n-12.4asd\n00\nU8 u 5\n98 56 1 9\n45\t");
  IPAdressPool ip_pool;
  ip_pool.read(is);
  auto pool = ip_pool.filter([](const IPAdress&){ return true; });
  std::cout << pool;

  EXPECT_TRUE(pool.empty());
}


TEST(IpFilterInputConversion, ValidInputDataTest) {
  std::istringstream is("43.48.1.9\n44.22.33.55\n");
  IPAdressPool ip_pool;
  ip_pool.read(is);
  auto pool = ip_pool.sort_reverse();
  auto ref_pool = std::vector<IPAdress>{{44, 22, 33, 55}, {43, 48, 1, 9}};

  EXPECT_EQ(pool, ref_pool);
}
