// GTest will be downloaded by CMake via FetchContent when the project is built
// If you see IntelliSense errors, run CMake first or install GTest locally
#include <gtest/gtest.h>

// Empty test to verify infrastructure works
TEST(InfrastructureTest, BasicAssertions) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}

// Placeholder for future tests
// When ready to add tests, create separate files:
// - file_test.cpp
// - mp3_test.cpp
// - id3v1_test.cpp
// - id3v2_test.cpp
