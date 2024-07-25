#include "../include/Game.hpp"
#include "../tests/OpenCVTests.h"
#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main() {
//    Game* game = Game::getInstance();
//    game->start();
//
//    Game::disposeInstance();
    openCvTest::openCvTest();
    return 0;
}
