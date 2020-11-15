// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>
#include "Tester.hpp"
TEST(Example, EmptyTest) {
  EXPECT_TRUE(true);
}

TEST(PROGRAMM, WORK){
  int L1 = 32;
  int L3 = 8192;//8193K =  8M

  Tester exp(L1, L3);

  std::stringstream str;
  exp.show_Experiment(str);
  std::cout << str.str();
  EXPECT_TRUE(true);
}

TEST(PROGRAMM, DEFINE_SERIES){
  int L1 = 32;
  int L3 = 8192;//8193K = 8M
  Tester exp(L1, L3);
  exp.define_series();
  std::stringstream str;
  exp.show_series(str);
  std::string ref_string = "16 32 64 128 256 512 1024 2048 4096 8192 12288 \n";

  EXPECT_EQ(str.str(), ref_string);
}

