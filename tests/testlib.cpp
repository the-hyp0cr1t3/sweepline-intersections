#include <gtest/gtest.h>

#include <project_name/lib.hpp>

TEST(HelloWorld, Basic){
    const int x = 2;
    EXPECT_EQ(2, x);
}