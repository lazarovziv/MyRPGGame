#include "Game.hpp"
#include "GameEntityMovement.hpp"
//#include "TextureLoader.hpp"

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
    
    state = GameState::PAUSED;
    
    int rows = 3;
    int cols = 3;
    worldMap = new GameMap**[rows];
    
    for (int r = 0; r < rows; r++) {
        worldMap[r] = new GameMap*[cols];
        for (int c = 0; c < cols; c++) {
            worldMap[r][c] = new GameMap(r, c);
            // cant go down but can go right, left and up
            if (r == rows - 1) {
                worldMap[r][c] = new GameMap(r, c, true, false, true, true);
                continue;
            }
            // cant go up but can go down, right, left
            if (r == 0) {
                worldMap[r][c] = new GameMap(r, c, false, true, true, true);
                continue;
            }
            // cant go right but can go down, up and left
            if (c == cols - 1) {
                worldMap[r][c] = new GameMap(r, c, true, true, false, true);
                continue;
            }
            // cant go left but can go right, up and down
            if (c == 0) {
                worldMap[r][c] = new GameMap(r, c, true, true, true, false);
                continue;

            }
            worldMap[r][c] = new GameMap(r, c, true, true, true, true);
        }
    }
    
    currentGameMapRow = 1;
    currentGameMapCol = 1;
    
//    changeCurrentMap(1, 1);
    
    this->player = new Player(PlayerType::KNIGHT);
    
    player->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    
    initWorldMap();
    // init first map
    getCurrentGameMap()->init();
}

void Game::render() {
    window->clear();
    GameMap* map = getCurrentGameMap();
    // drawing unreachable areas
    for (int i = 0; i < map->getNumOfUnreachableAreas(); i++) {
        window->draw(map->getUnreachableAreasSprites()[i]);
    }
    for (int i = 0; i < map->getNumOfCurrentEnemies(); i++) {
        if (!map->getEnemies0()[i].isDead()) {
            window->draw(map->getEnemies0()[i].getSprite());
        }
    }
    window->draw(player->getSprite());
    window->display();
}

void Game::start() {
    player->increaseSpeed(7);
    GameEntityMovement playerMovement(player);
    
    bool canMove = false;
    
    // game loop
    while (window->isOpen()) {
        GameMap* map = getCurrentGameMap();
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
                        // pressing x for attacking
                    } else if (eventKeyCode == Keyboard::X) {
                        for (int i = 0; i < map->getNumOfCurrentEnemies(); i++) {
                            if (!map->getEnemies0()[i].isDead()) {
                                player->attack(map->getEnemies0()[i]);
                            }
                        }
                    }
                    if (canMove) {
//                        for (int i = 0; i < map->getEnemies().size(); i++) {
//                            if (map->getEnemies()[i] == nullptr) continue;
//                        }
                    }
                    // update player data
                    update();
                } else if (state == GameState::IN_MENU) {
                    // exiting menu by pressing I again
                    if (eventKeyCode == Keyboard::I) {
                        state = GameState::PLAYING;
                        cout << "Exited Menu" << endl;
                        canMove = true;
                    }
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
    // updating player state
    player->update();
    // updating enemies states
    GameMap* map = getCurrentGameMap();
    for (int i = 0; i < map->getNumOfCurrentEnemies(); i++) {
        // setting dead enemy in last index of array and subtracting size
        if (map->getEnemies0()[i].isDead()) {
            // swapping current with last and popping last element (erase doesn't work well for some reason)
            map->removeEnemyAtIndex(i);
//            std::swap(map->getEnemies()[i], map->getEnemies()[map->getEnemies().size() - 1]);
//            map->getEnemies().pop_back();
            continue;
        }
        // if enemy is still alive
        map->getEnemies0()[i].update();
    }
}

void Game::initWorldMap() {
    // TODO: declare all maps here with unreachable areas and exit/enter points
    GameMap* map = worldMap[currentGameMapRow][currentGameMapRow];
    map->setTopExitMinX(400);
    map->setTopExitMaxX(500);
    map->setTopEnterMinX(400);
    map->setTopEnterMaxX(500);
    
    FloatRect unreachableArea0(200, 100, 100, 100);
    FloatRect unreachableArea1(400, 400, 100, 100);
    map->addUnreachableArea(unreachableArea0);
    map->addUnreachableArea(unreachableArea1);
    
    GameMap* mapTop = worldMap[currentGameMapRow - 1][currentGameMapCol];
    FloatRect unreachableArea2(100, 150, 100, 100);
    FloatRect unreachableArea3(200, 400, 100, 100);
    mapTop->addUnreachableArea(unreachableArea2);
    mapTop->addUnreachableArea(unreachableArea3);
    
    mapTop->setBottomEnterMinX(400);
    mapTop->setBottomEnterMaxX(500);
    mapTop->setBottomExitMinX(400);
    mapTop->setBottomExitMaxX(500);
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

void Game::changeCurrentMap(int row, int col) {
    GameMap* map = getCurrentGameMap();
    // delete enemies because current map has changed
    delete [] map->getEnemies0();
    delete [] map->getEnemies0();
//    map->getEnemies().clear();
    
    setCurrentWorldMapRow(row);
    setCurrentWorldMapCol(col);
    // initialize map
//    worldMap[currentGameMapRow][currentGameMapCol]->init();
}
