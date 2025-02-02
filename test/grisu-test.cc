// Formatting library for C++ - Grisu tests
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#include "fmt/format.h"
#include <gmock/gmock.h>

static bool reported_skipped;

#undef TEST
#define TEST(test_fixture, test_name)        \
  void test_fixture##test_name();            \
  GTEST_TEST(test_fixture, test_name) {      \
    if (FMT_USE_GRISU) {                     \
      test_fixture##test_name();             \
    } else if (!reported_skipped) {          \
      reported_skipped = true;               \
      fmt::print("Skipping Grisu tests.\n"); \
    }                                        \
  }                                          \
  void test_fixture##test_name()

TEST(GrisuTest, NaN) {
  auto nan = std::numeric_limits<double>::quiet_NaN();
  EXPECT_EQ("nan", fmt::format("{}", nan));
  EXPECT_EQ("-nan", fmt::format("{}", -nan));
}

TEST(GrisuTest, Inf) {
  auto inf = std::numeric_limits<double>::infinity();
  EXPECT_EQ("inf", fmt::format("{}", inf));
  EXPECT_EQ("-inf", fmt::format("{}", -inf));
}

TEST(GrisuTest, Zero) { EXPECT_EQ("0.0", fmt::format("{}", 0.0)); }

TEST(GrisuTest, Round) {
  EXPECT_EQ("1.9156918820264798e-56",
            fmt::format("{}", 1.9156918820264798e-56));
  EXPECT_EQ("0.0000", fmt::format("{:.4f}", 7.2809479766055470e-15));
}

TEST(GrisuTest, Prettify) {
  EXPECT_EQ("0.0001", fmt::format("{}", 1e-4));
  EXPECT_EQ("1e-05", fmt::format("{}", 1e-5));
  EXPECT_EQ("9.999e-05", fmt::format("{}", 9.999e-5));
  EXPECT_EQ("10000000000.0", fmt::format("{}", 1e10));
  EXPECT_EQ("1e+11", fmt::format("{}", 1e11));
  EXPECT_EQ("12340000000.0", fmt::format("{}", 1234e7));
  EXPECT_EQ("12.34", fmt::format("{}", 1234e-2));
  EXPECT_EQ("0.001234", fmt::format("{}", 1234e-6));
}

TEST(GrisuTest, ZeroPrecision) { EXPECT_EQ("1", fmt::format("{:.0}", 1.0)); }
