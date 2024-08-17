#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include "../../../MyRPGGame/core/include/NavigateFiles.hpp"


TEST(StringComparisonTest, StringTest){
    std::string expectedResult = "../../../graphics/Characters/Head/masculine/climb.png";
    std::string actualResult = navToRoot("graphics/Characters/Head/masculine/climb.png");
    EXPECT_EQ(actualResult,expectedResult);
}