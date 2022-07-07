#include "Game.hpp"
#include "GameEntityMovement.hpp"

using namespace std;

Game::Game(const char* str) {
    title = str;
    this->videoMode = new VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
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
    
    this->player = new Player();
    
    player->setCurrentGameMap(*(worldMap[0][0]));
    player->setPosition(SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT/2 - 100);
    
    FloatRect unreachableArea0(200, 100, 100, 100);
    FloatRect unreachableArea1(300, 400, 100, 100);
    worldMap[0][0]->addUnreachableArea(unreachableArea0);
    worldMap[0][0]->addUnreachableArea(unreachableArea1);
//    cout << worldMap[0][0]->getNumOfUnreachableAreas() << endl;
}

void Game::render() {
    window->clear();
    RectangleShape rect0(Vector2f(100, 100));
    rect0.setPosition(200, 100);
    RectangleShape rect1(Vector2f(100, 100));
    rect1.setPosition(300, 400);
    window->draw(rect0);
    window->draw(rect1);
    window->draw(player->getSprite());
    window->display();
}

void Game::start() {
//    player->increaseSpeed(23);
    GameEntityMovement playerMovement(player);
    
    bool canMove = false;
    
    // game loop
    while (window->isOpen()) {
        Event event;
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                // add save game and exit message confirmation
                window->close();
            }
            if (event.type == Event::KeyPressed) {
                int eventKeyCode = event.key.code;
                // exit the game
                if (eventKeyCode == Keyboard::Escape) {
                    window->close();
                    // starting the game by pressing enter
                } else if (eventKeyCode == Keyboard::Enter) {
                    changeState(GameState::PLAYING);
                    canMove = true;
                }
                if (state == GameState::PLAYING) {
                    // moving with the arrows
                    if (eventKeyCode == Keyboard::Up && canMove) {
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
                    if (canMove) cout << "Player (x, y): (" << player->getPosition().x << ", " << player->getPosition().y << ")" << endl;
                    // update player data
                    update();
                }
            }
        }
        
        // redner only when playing
        if (state == GameState::PLAYING) {
            render();
        }
    }
}

void Game::changeState(GameState state) {
    this->state = state;
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
    player->update();
}
