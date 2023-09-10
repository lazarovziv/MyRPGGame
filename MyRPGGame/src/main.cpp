#include "../include/Game.hpp"

using namespace std;
using namespace sf;

int main() {
    Game* game = Game::getInstance();
    game->start();

    Game::disposeInstance();

    return 0;
}
