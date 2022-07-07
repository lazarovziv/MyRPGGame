#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "GameMap.hpp"
#include "Player.hpp"
#include "GameEntityMovement.hpp"

using namespace std;
using namespace sf;

int main() {
    Game game("MyRPGGame");
    game.start();
    
    return 0;
}
