#include <gtest/gtest.h>

#include "test/greeter.h"

TEST(GreeterTest, GreetsByName) {
    EXPECT_EQ(test::greet("world"), "Hello, world!");
}

TEST(GreeterTest, HandlesEmptyName) {
    EXPECT_EQ(test::greet(""), "Hello, !");
}
