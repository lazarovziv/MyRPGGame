#include <iostream>
#include <gtest/gtest.h>
#include "../../../MyRPGGame/core/include/generateImg.hpp"

void openCvTest() {
   std::string first ="graphics/Characters/Head/masculine/climb.png";
   std::string second = "graphics/Characters/Body/masculine/climb.png";

    alphaBlend(first,second,"blended_img.png");
}

TEST(first, second){
    openCvTest();
}


