#include "Game.hpp"
#include "GameEntityMovement.hpp"

using namespace std;

Game* Game::instance = nullptr;

Game* Game::getInstance() {
    if (instance == nullptr) {
        instance = new Game("MyRPGGame");
    }
    return instance;
}

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
    
    
//    player->setCurrentGameMap(*(worldMap[0][0]));
    player->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    
    GameMap* map = worldMap[currentGameMapRow][currentGameMapRow];
    map->setTopExitMinX(400);
    map->setTopExitMaxX(500);
    map->setIsExitableFromTop(true);
    
    FloatRect unreachableArea0(200, 100, 100, 100);
    FloatRect unreachableArea1(400, 400, 100, 100);
    map->addUnreachableArea(unreachableArea0);
    map->addUnreachableArea(unreachableArea1);
    
    GameMap* mapTop = worldMap[currentGameMapRow + 1][currentGameMapCol];
    FloatRect unreachableArea2(200, 100, 100, 100);
    FloatRect unreachableArea3(200, 400, 100, 100);
    mapTop->addUnreachableArea(unreachableArea2);
    mapTop->addUnreachableArea(unreachableArea3);
    
    mapTop->setBottomEnterMinX(400);
    mapTop->setBottomEnterMaxX(500);
}

void Game::render() {
    window->clear();
    GameMap* map = getCurrentGameMap();
    // drawing unreachable areas
    for (int i = 0; i < map->getNumOfUnreachableAreas(); i++) {
        window->draw(map->getUnreachableAreasSprites()[i]);
    }
    window->draw(player->getSprite());
    window->display();
}

void Game::start() {
    player->increaseSpeed(3);
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

int Game::getCurrentWorldMapRow() {
    return currentGameMapRow;
}

int Game::getCurrentWorldMapCol() {
    return currentGameMapCol;
}

void Game::setCurrentWorldMapRow(int row) {
    currentGameMapRow = row;
}

void Game::setCurrentWorldMapCol(int col) {
    currentGameMapCol = col;
}
