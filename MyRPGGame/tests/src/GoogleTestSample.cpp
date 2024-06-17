#include <gtest/gtest.h>
#include <Vector.hpp>

// Demonstrate some basic assertions.
TEST(SampleTestsGroupName, SampleTestName) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
    // should fail
    EXPECT_EQ(4 * 5, 20);
}

TEST(SampleTestsGroupName, SampleTestName1) {
    EXPECT_EQ((physics::Vector{0, 1, 1.5}), (physics::Vector{0, 1, 1.5000}));
}
