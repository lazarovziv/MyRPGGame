#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "GameMap.hpp"
#include "Player.hpp"
#include "GameEntityMovement.hpp"

using namespace std;
using namespace sf;

int main() {
    Game game("MyRPGGame", 320, 240);
    Player player(&game);
    
    player.increaseSpeed(1.5f);
    player.setPosition(100, 100);
    
    GameEntityMovement playerMovement(&game, &player, game.getCurrentGameMap());
    
    bool canMove = false;
    
    // game loop
    while (game.getWindow()->isOpen()) {
        Event event;
        while (game.getWindow()->pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                int eventKeyCode = event.key.code;
                // exit the game
                if (eventKeyCode == Keyboard::Escape) {
                    game.getWindow()->close();
                    // starting the game by pressing enter
                } else if (eventKeyCode == Keyboard::Return) {
                    game.changeState(GameState::PLAYING);
                    canMove = true;
                    // moving with the arrows
                } else if (eventKeyCode == Keyboard::Up && canMove) {
                    playerMovement.move(MoveDirection::UP);
                } else if (eventKeyCode == Keyboard::Down && canMove) {
                    playerMovement.move(MoveDirection::DOWN);
                } else if (eventKeyCode == Keyboard::Right && canMove) {
                    playerMovement.move(MoveDirection::RIGHT);
                } else if (eventKeyCode == Keyboard::Left && canMove) {
                    playerMovement.move(MoveDirection::LEFT);
                    // pressing I sends to menu (not implemented menu yet)
                } else if (eventKeyCode == Keyboard::I) {
                    game.changeState(GameState::IN_MENU);
                    cout << "In Menu" << endl;
                    canMove = false;
                }
                if (canMove) cout << "Player: (" << player.getPosition().x << ", " << player.getPosition().y << ")" << endl;
            }
        }
        
        // redner only when playing
        if (game.getState() == GameState::PLAYING) {
            game.getWindow()->clear();
            game.getWindow()->draw(player.getSprite());
            game.getWindow()->display();
        }
        
//        // updating game state
//        game.update();
//        // rendering game graphics
//        game.render();
    }
    return 0;
}
