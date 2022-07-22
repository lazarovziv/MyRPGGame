#include <iostream>
#include "Game.hpp"

using namespace std;
using namespace sf;

int main() {
    Game* game = Game::getInstance();
    game->start();
    
    return 0;
}
