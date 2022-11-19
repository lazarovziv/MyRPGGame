#include "Game.hpp"
#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"
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
    
    this->player = new Player(PlayerType::KNIGHT);
    
    player->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/4);
    
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
    // drawing undead enemies
    for (int i = 0; i < map->getEnemiesVector().size(); i++) {
        if (!map->getEnemiesVector()[i].isDead()) {
            window->draw(*(map->getEnemiesVector()[i].getSprite()));
        }
    }
    window->draw(*(player->getSprite()));
    window->display();
}

void Game::start() {
    cout << "Press Enter to start" << endl;
    cout << "Press I to enter menu" << endl;
    cout << "Press X near an enemy to attack" << endl;
    // initialize player's systems
    player->increaseSpeed(13);
    GameEntityMovement playerMovement(player);
    GameEntityBattle playerBattle(player);
    
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
                    bool moved = false;
                    // moving with the arrows
                    if (eventKeyCode == Keyboard::Up && canMove) {
                        moved = playerMovement.move(MoveDirection::UP);
                    } else if (eventKeyCode == Keyboard::Down && canMove) {
                        moved = playerMovement.move(MoveDirection::DOWN);
                    } else if (eventKeyCode == Keyboard::Right && canMove) {
                        moved = playerMovement.move(MoveDirection::RIGHT);
                    } else if (eventKeyCode == Keyboard::Left && canMove) {
                        moved = playerMovement.move(MoveDirection::LEFT);
                        // pressing I sends to menu (not implemented menu yet)
                    } else if (eventKeyCode == Keyboard::I) {
                        changeState(GameState::IN_MENU);
                        cout << "In Menu (Press Enter to exit menu)" << endl;
                        canMove = false;
                        // pressing x for attacking
                    } else if (eventKeyCode == Keyboard::X) {
                        for (int i = 0; i < map->getEnemiesVector().size(); i++) {
                            if (!map->getEnemiesVector()[i].isDead()) {
                                playerBattle.attack(map->getEnemiesVector()[i]);
                            }
                        }
                    }
                    // do something if player moved
                    if (moved) {
//                        cout << "Moved" << endl;
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
    for (int i = 0; i < map->getEnemiesVector().size(); i++) {
        // checking if enemy is dead
        if (map->getEnemiesVector()[i].isDead()) {
            // remove it from currentEnemies
            map->removeEnemyAtIndexFromVector(i);
            continue;
        }
        // if enemy is still alive
        map->getEnemiesVector()[i].update();
    }
}

void Game::initWorldMap() {
    // TODO: declare all maps here with unreachable areas and exit/enter points
    GameMap* map = worldMap[currentGameMapRow][currentGameMapRow];
    map->setTopExit(SCREEN_WIDTH/2, SCREEN_WIDTH/2 + 2*TILE_SIZE);
    //map->setTopExit(400, 500);
//    map->setTopExitMinX(400);
//    map->setTopExitMaxX(500);

    map->setTopEnterMinX(SCREEN_WIDTH/2);
    map->setTopEnterMaxX(SCREEN_WIDTH/2 + 2*TILE_SIZE);
    //map->setTopEnterMinX(400);
    //map->setTopEnterMaxX(500);
    
    FloatRect unreachableArea0(SCREEN_WIDTH/4, SCREEN_HEIGHT/6, SCREEN_WIDTH/8, SCREEN_HEIGHT/6);
    FloatRect unreachableArea1(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/8, SCREEN_HEIGHT/6);
    // FloatRect unreachableArea0(200, 100, 100, 100);
    // FloatRect unreachableArea1(400, 400, 100, 100);
    map->addUnreachableArea(unreachableArea0);
    map->addUnreachableArea(unreachableArea1);
    
    GameMap* mapTop = worldMap[currentGameMapRow - 1][currentGameMapCol];
    FloatRect unreachableArea2(SCREEN_WIDTH/8, SCREEN_HEIGHT/4, SCREEN_WIDTH/8, SCREEN_HEIGHT/6);
    FloatRect unreachableArea3(SCREEN_WIDTH/4, 2*SCREEN_HEIGHT/3, SCREEN_WIDTH/8, SCREEN_HEIGHT/6);
    // FloatRect unreachableArea2(100, 150, 100, 100);
    // FloatRect unreachableArea3(200, 400, 100, 100);
    mapTop->addUnreachableArea(unreachableArea2);
    mapTop->addUnreachableArea(unreachableArea3);
    
    mapTop->setBottomEnterMinX(SCREEN_WIDTH/2);
    mapTop->setBottomEnterMaxX(SCREEN_WIDTH/2 + 2*TILE_SIZE);
    // mapTop->setBottomEnterMinX(400);
    // mapTop->setBottomEnterMaxX(500);
    mapTop->setBottomExit(SCREEN_WIDTH/2, SCREEN_WIDTH/2 + 2*TILE_SIZE);
    // mapTop->setBottomExit(400, 500);
//    mapTop->setBottomExitMinX(400);
//    mapTop->setBottomExitMaxX(500);
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
//    GameMap* map = getCurrentGameMap();
    // delete enemies because current map has changed
    getCurrentGameMap()->removeAllEnemies();
    // change current map
    setCurrentWorldMapRow(row);
    setCurrentWorldMapCol(col);
    // initialize map
    worldMap[currentGameMapRow][currentGameMapCol]->init();
}
