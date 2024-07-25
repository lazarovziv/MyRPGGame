#include "../include/Game.hpp"

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main() {
    // Game* game = Game::getInstance();
    Game::get_instance()->start();

    // Game::disposeInstance();

    return 0;
}
