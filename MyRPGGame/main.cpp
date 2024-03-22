#include "main/include/Game.hpp"

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main() {
    Game* game = Game::getInstance();
    game->start();

    Game::disposeInstance();

    return 0;
}
