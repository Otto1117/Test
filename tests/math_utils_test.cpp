#include <gtest/gtest.h>

#include <climits>

#include "test/math_utils.h"

TEST(AddTest, AddsPositiveNumbers) {
    EXPECT_EQ(test::add(2, 3), 5);
}

TEST(AddTest, AddsNegativeNumbers) {
    EXPECT_EQ(test::add(-2, -3), -5);
}

TEST(AddTest, AddsMixedSigns) {
    EXPECT_EQ(test::add(-2, 5), 3);
    EXPECT_EQ(test::add(5, -2), 3);
}

TEST(AddTest, ZeroIsIdentity) {
    EXPECT_EQ(test::add(0, 0), 0);
    EXPECT_EQ(test::add(7, 0), 7);
    EXPECT_EQ(test::add(0, 7), 7);
}

TEST(AddTest, IsCommutative) {
    EXPECT_EQ(test::add(12, 34), test::add(34, 12));
}

TEST(AddTest, HandlesLimitsWithoutOverflow) {
    EXPECT_EQ(test::add(INT_MAX, 0), INT_MAX);
    EXPECT_EQ(test::add(INT_MAX, -1), INT_MAX - 1);
    EXPECT_EQ(test::add(INT_MIN, 1), INT_MIN + 1);
    EXPECT_EQ(test::add(INT_MAX, INT_MIN), -1);
}
