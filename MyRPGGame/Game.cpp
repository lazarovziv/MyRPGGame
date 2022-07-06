#include "Game.hpp"
#include "GameEntityMovement.hpp"

using namespace std;

Game::Game(const char* str, int width, int height) {
    screenWidth = width;
    screenHeight = height;
    title = str;
    this->videoMode = new VideoMode(screenWidth, screenHeight);
    std::string s(title);
    this->window = new RenderWindow(*videoMode, s);
    
    state = GameState::IN_MENU;
    
    currentGameMapRow = 0;
    currentGameMapCol = 0;
    
    worldMap = new GameMap**[3];
    for (int r = 0; r < 3; r++) {
        worldMap[r] = new GameMap*[3];
        for (int c = 0; c < 3; c++) {
            worldMap[r][c] = new GameMap(r, c);
        }
    }
}

void Game::changeState(GameState state) {
    this->state = state;
}

int Game::getScreenWidth() {
    return screenWidth;
}

int Game::getScreenHeight() {
    return screenHeight;
}

RenderWindow* Game::getWindow() {
    return window;
}

GameState Game::getState() {
    return state;
}

GameMap*** Game::getWorldMap() {
    return worldMap;
}

GameMap* Game::getCurrentGameMap() {
    return worldMap[currentGameMapRow][currentGameMapCol];
}

void Game::setPlayer(Player* player) {
    this->player = player;
}

void Game::update() {
    
}

void Game::render() {
    window->clear();
    window->draw(player->getSprite());
    window->display();
}

void Game::start() {
    GameEntityMovement playerMovement(player);
    
    bool canMove = false;
    
    // game loop
    while (window->isOpen()) {
        Event event;
        while (window->pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                int eventKeyCode = event.key.code;
                // exit the game
                if (eventKeyCode == Keyboard::Escape) {
                    window->close();
                    // starting the game by pressing enter
                } else if (eventKeyCode == Keyboard::Return) {
                    changeState(GameState::PLAYING);
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
                    changeState(GameState::IN_MENU);
                    cout << "In Menu" << endl;
                    canMove = false;
                }
                if (canMove) cout << "Player: (" << player->getPosition().x << ", " << player->getPosition().y << ")" << endl;
            }
        }
        
        // redner only when playing
        if (state == GameState::PLAYING) {
            render();
        }
        
//        // updating game state
//        game.update();
//        // rendering game graphics
//        game.render();
    }
}
