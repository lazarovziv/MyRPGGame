#include "../include/Game.hpp"
#include "../include/GameEntityMovement.hpp"
#include "../include/GameEntityBattle.hpp"
//#include "TextureLoader.hpp"

using namespace std;

Game* Game::instance = nullptr;

Game* Game::getInstance() {
    if (instance == nullptr) {
        std::cout << "Initializing game..." << endl;
        instance = new Game("MyRPGGame");
    }
    return instance;
}

void Game::disposeInstance() {
    delete instance;
    instance = nullptr;
}

Game::~Game() {
    if (player) delete player;
    if (worldMap) delete worldMap;
    if (window) delete window;
}

Game::Game(const char* str) {
    title = str;
    VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
    std::string s(title);
    window = new RenderWindow(videoMode, s);
    
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
    // draw background
    window->draw(*(map->getBackgroundSprite()));
    // drawing unreachable areas
    for (int i = 0; i < map->getLandscapes().size(); i++) {
        window->draw(*(map->getLandscapes()[i]->getSprite()));
    }
    // drawing undead enemies
    for (int i = 0; i < map->getEnemies().size(); i++) {
        if (!map->getEnemies()[i]->isDead()) {
            window->draw(*(map->getEnemies()[i]->getSprite()));
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
//    player->increaseSpeed(13);
    GameEntityMovement* playerMovement = new GameEntityMovement(player, true);
    GameEntityBattle* playerBattle = new GameEntityBattle(player);
    GameEntityMovement* enemiesMovement = new GameEntityMovement(nullptr, false);
    
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
                        moved = playerMovement->move(MoveDirection::UP);
                    } else if (eventKeyCode == Keyboard::Down && canMove) {
                        moved = playerMovement->move(MoveDirection::DOWN);
                    } else if (eventKeyCode == Keyboard::Right && canMove) {
                        moved = playerMovement->move(MoveDirection::RIGHT);
                    } else if (eventKeyCode == Keyboard::Left && canMove) {
                        moved = playerMovement->move(MoveDirection::LEFT);
                        // pressing I sends to menu (not implemented menu yet)
                    } else if (eventKeyCode == Keyboard::I) {
                        changeState(GameState::IN_MENU);
                        cout << "In Menu (Press Enter to exit menu)" << endl;
                        canMove = false;
                        // pressing x for attacking
                    } else if (eventKeyCode == Keyboard::X) {
                        for (int i = 0; i < map->getEnemies().size(); i++) {
                            if (!map->getEnemies()[i]->isDead()) {
                                playerBattle->attack(*(map->getEnemies()[i]));
                            }
                        }
                    }
                    // do something if player moved
                    if (moved) {
                        // TODO: make enemies drawn to player???
                        for (int i = 0; i < map->getEnemies().size(); i++) {
                            if (map->getEnemies()[i]->isInBattle()) {
                                // if enemy not in range of player's attacks
                                if (!player->getAttackRangeCircle()->intersects(map->getEnemies()[i]->getCircle())) {
                                    map->getEnemies()[i]->setIsInBattle(false);
                                }
                            }
                        }
                    }
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

        if (state == GameState::PLAYING) {
            // make enemies move
            for (int i = 0; i < map->getEnemies().size(); i++) {
                if (!map->getEnemies().at(i)->isDead() && map->getEnemies().at(i)->canMove() && !map->getEnemies().at(i)->isInBattle()) {
                    // set enemy if not already set
                    if (enemiesMovement->getEntity() != map->getEnemies().at(i)) enemiesMovement->setEntity(*(map->getEnemies().at(i)));
                    // choose random direction
                    int randomDirection = (int) rand() % 4;
                    // DOWN, RIGHT, LEFT, UP
                    switch (randomDirection) {
                        case 0:
                            enemiesMovement->move(MoveDirection::DOWN);
                            break;
                        case 1:
                            enemiesMovement->move(MoveDirection::RIGHT);
                            break;
                        case 2:
                            enemiesMovement->move(MoveDirection::LEFT);
                            break;
                        case 3:
                            enemiesMovement->move(MoveDirection::UP);
                            break;
                    }
                }
            }
            update();
            // render only when playing
            render();
        }
    }

    // deleting all maps from world map
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            delete worldMap[i][j];
        }
        delete worldMap[i];
    }

    delete playerMovement;
    delete enemiesMovement;
    delete playerBattle;
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
    for (int i = 0; i < map->getEnemies().size(); i++) {
        // checking if enemy is dead
        if (map->getEnemies()[i]->isDead()) {
            // remove it from currentEnemies
            map->removeEnemyAtIndex(i);
            continue;
            // if enemy is still alive
        } else map->getEnemies()[i]->update();
    }
}

void Game::initWorldMap() {
    // TODO: declare all maps here with unreachable areas and exit/enter points
    GameMap* map = worldMap[currentGameMapRow][currentGameMapRow];
    // setting exit and enter points
    map->setTopExit(SCREEN_WIDTH/2, SCREEN_WIDTH/2 + 2*TILE_SIZE);
    map->setTopEnterMinX(SCREEN_WIDTH/2);
    map->setTopEnterMaxX(SCREEN_WIDTH/2 + 2*TILE_SIZE);
    // adding unreachable areas and landscapes
    LandscapeEntity* unreachableTree0 = new LandscapeEntity(LandscapeType::TREE, 3*(SCREEN_WIDTH/16), SCREEN_HEIGHT/6);
    LandscapeEntity* unreachableTree1 = new LandscapeEntity(LandscapeType::TREE, 5*(SCREEN_WIDTH/16), SCREEN_HEIGHT/3);
    map->addLandscape(unreachableTree0);
    map->addLandscape(unreachableTree1);
    
    GameMap* mapTop = worldMap[currentGameMapRow - 1][currentGameMapCol];
    // setting exit and enter points
    mapTop->setBottomEnterMinX(SCREEN_WIDTH/2);
    mapTop->setBottomEnterMaxX(SCREEN_WIDTH/2 + 2*TILE_SIZE);
    mapTop->setBottomExit(SCREEN_WIDTH/2, SCREEN_WIDTH/2 + 2*TILE_SIZE);
    // adding unreachable areas and landscapes
    LandscapeEntity* unreachableTree2 = new LandscapeEntity(LandscapeType::TREE, SCREEN_WIDTH/8, 5*(SCREEN_HEIGHT/24));
    LandscapeEntity* unreachableTree3 = new LandscapeEntity(LandscapeType::TREE, 3*(SCREEN_WIDTH/16), 5*(SCREEN_HEIGHT/12));
    mapTop->addLandscape(unreachableTree2);
    mapTop->addLandscape(unreachableTree3);


    // deallocate memory if needed
}

int Game::getCurrentWorldMapRow() {
    return currentGameMapRow;
}

int Game::getCurrentWorldMapCol() {
    return currentGameMapCol;
}

Player* Game::getPlayer() {
    return player;
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
//    getCurrentGameMap()->removeAllEnemies();
    // change current map
    setCurrentWorldMapRow(row);
    setCurrentWorldMapCol(col);
    // initialize map
    worldMap[currentGameMapRow][currentGameMapCol]->init();
}
