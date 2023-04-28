#include "../include/Game.hpp"
//#include "TextureLoader.hpp"

//using namespace std;

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
    for (int row = 0; row < Constants::NUM_ROWS; row++) {
        for (int col = 0; col < Constants::NUM_COLS; col++) {
            delete worldMap[row][col];
        }
        delete[] worldMap[row];
    }
    delete player;
    delete worldMap;
    delete window;
    delete title;
}

Game::Game(const char* str) {
    title = str;
    VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
    std::string s(title);
    window = new RenderWindow(videoMode, s);

    window->setVerticalSyncEnabled(true);
//    window->setFramerateLimit(Constants::FPS);
    window->setFramerateLimit(0);

    graph = new Graph<Point *>();

    state = GameState::PAUSED;

    // init all possible points
    points = new Point **[Constants::SCREEN_HEIGHT];
    for (int row = 0; row < Constants::SCREEN_HEIGHT; row++) {
        points[row] = new Point *[Constants::SCREEN_WIDTH];
        for (int col = 0; col < Constants::SCREEN_WIDTH; col++) {
            points[row][col] = new Point(col, row);
        }
    }

    // init world map
    worldMap = new GameMap **[Constants::NUM_ROWS];
    for (int r = 0; r < Constants::NUM_ROWS; r++) {
        worldMap[r] = new GameMap *[Constants::NUM_COLS];
        for (int c = 0; c < Constants::NUM_COLS; c++) {
            // cant go down but can go right, left and up
            if (r == Constants::NUM_ROWS - 1) {
                worldMap[r][c] = new GameMap(r, c, true, false, true, true, points);
                continue;
            }
            // cant go up but can go down, right, left
            if (r == 0) {
                worldMap[r][c] = new GameMap(r, c, false, true, true, true, points);
                continue;
            }
            // cant go right but can go down, up and left
            if (c == Constants::NUM_COLS - 1) {
                worldMap[r][c] = new GameMap(r, c, true, true, false, true, points);
                continue;
            }
            // cant go left but can go right, up and down
            if (c == 0) {
                worldMap[r][c] = new GameMap(r, c, true, true, true, false, points);
                continue;
            }
            worldMap[r][c] = new GameMap(r, c, true, true, true, true, points);
        }
    }

    currentGameMapRow = 1;
    currentGameMapCol = 1;
    
    this->player = new Player(PlayerType::KNIGHT, points[SCREEN_HEIGHT/2][SCREEN_WIDTH/2]);
    
    initWorldMap();
    // init first map
    getCurrentGameMap()->init();

    bool occupied = false;
    // add vertices (in relevant points)
    for (int x = 0; x <= Constants::SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED; x += Constants::BASE_ENTITY_SPEED) {
        for (int y = 0; y <= Constants::SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED; y += Constants::BASE_ENTITY_SPEED) {
            graph->addVertex(points[x][y]);
        }
    }

    // add edges
    bool toAdd = true;
    for (int x = Constants::BASE_ENTITY_SPEED; x <= Constants::SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED; x += Constants::BASE_ENTITY_SPEED) {
        for (int y = Constants::BASE_ENTITY_SPEED; y <= Constants::SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED; y += Constants::BASE_ENTITY_SPEED) {
            toAdd = true;
            // down, right, left
            if (x == Constants::BASE_ENTITY_SPEED) {
                graph->addEdge(points[x][y], points[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                graph->addEdge(points[x][y], points[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                graph->addEdge(points[x][y], points[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
            // up, right, left
            else if (x == Constants::SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED) {
                graph->addEdge(points[x][y], points[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                graph->addEdge(points[x][y], points[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                graph->addEdge(points[x][y], points[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
            // up, down, left
            else if (y == Constants::SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED) {
                graph->addEdge(points[x][y], points[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                graph->addEdge(points[x][y], points[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                graph->addEdge(points[x][y], points[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
            // up, down, right
            else if (y == Constants::BASE_ENTITY_SPEED) {
                graph->addEdge(points[x][y], points[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                graph->addEdge(points[x][y], points[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                graph->addEdge(points[x][y], points[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                toAdd = false;
            }

            // up, down, right, left
            if (toAdd) {
                graph->addEdge(points[x][y], points[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                graph->addEdge(points[x][y], points[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                graph->addEdge(points[x][y], points[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                graph->addEdge(points[x][y], points[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
            }
        }
    }

    // remove all vertices which are occupied by landscapes
    for (auto &landscape : getCurrentGameMap()->getLandscapes()) {
        for (int row = landscape->getCircle()->getCenter()->getX();
             row < landscape->getCircle()->getCenter()->getX() + TILE_SIZE/4; row++) {
            for (int col = landscape->getCircle()->getCenter()->getY();
                 col < landscape->getCircle()->getCenter()->getY() + TILE_SIZE/4; col++) {
                graph->removeVertex(points[row][col]);
            }
        }
    }

    cout << "START!" << endl;
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
    auto *playerMovement = new GameEntityMovement(player, true, getCurrentGameMap(), points);
    auto *playerBattle = new GameEntityBattle(player);
    auto *enemiesMovement = new GameEntityMovement(nullptr, false, getCurrentGameMap(), points);
    
    bool canMove = false;
    int running = window->isOpen();

    GameMap *map;
    Event event;

    int eventKeyCode;
    bool moved = false;

    // game loop
    while (/*window->isOpen() && */running) {
        map = getCurrentGameMap();
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                state = GameState::EXITING;
                // add save game and exit message confirmation
//                window->close();
                running = false;
            }

            if (event.type == Event::KeyPressed) {
                eventKeyCode = event.key.code;
                // exit the game
                if (eventKeyCode == Keyboard::Escape) {
                    state = GameState::EXITING;
//                    window->close();
                    running = false;
                    // starting the game by pressing enter
                } else if (eventKeyCode == Keyboard::Enter) {
                    changeState(GameState::PLAYING);
                    canMove = true;
                }
                if (state == GameState::PLAYING) {
                    // moving with the arrows
                    if (eventKeyCode == Keyboard::Up && canMove) {
                        // adding diagonal movement
                        if (Keyboard::isKeyPressed(Keyboard::Right)) moved = playerMovement->move(MoveDirection::UP_RIGHT);
                        else if (Keyboard::isKeyPressed(Keyboard::Left)) moved = playerMovement->move(MoveDirection::UP_LEFT);
                        // no diagonal movement
                        else moved = playerMovement->move(MoveDirection::UP);

                    } else if (eventKeyCode == Keyboard::Down && canMove) {
                        // adding diagonal movement
                        if (Keyboard::isKeyPressed(Keyboard::Right)) moved = playerMovement->move(MoveDirection::DOWN_RIGHT);
                        else if (Keyboard::isKeyPressed(Keyboard::Left)) moved = playerMovement->move(MoveDirection::DOWN_LEFT);
                        else moved = playerMovement->move(MoveDirection::DOWN);

                    } else if (eventKeyCode == Keyboard::Right && canMove) {
                        // adding diagonal movement
                        if (Keyboard::isKeyPressed(Keyboard::Up)) moved = playerMovement->move(MoveDirection::UP_RIGHT);
                        else if (Keyboard::isKeyPressed(Keyboard::Down)) moved = playerMovement->move(MoveDirection::DOWN_RIGHT);
                        else moved = playerMovement->move(MoveDirection::RIGHT);

                    } else if (eventKeyCode == Keyboard::Left && canMove) {
                        // adding diagonal movement
                        if (Keyboard::isKeyPressed(Keyboard::Up)) moved = playerMovement->move(MoveDirection::UP_LEFT);
                        else if (Keyboard::isKeyPressed(Keyboard::Down)) moved = playerMovement->move(MoveDirection::DOWN_LEFT);
                        // no diagonal movement
                        else moved = playerMovement->move(MoveDirection::LEFT);
                    }

                    // pressing I sends to menu (not implemented menu yet)
                    if (eventKeyCode == Keyboard::I) {
                        changeState(GameState::IN_MENU);
                        cout << "In Menu (Press Enter to exit menu)" << endl;
                        canMove = false;
                        // pressing x for attacking
                    }
                    // attacking
                    if (eventKeyCode == Keyboard::X) {
                        for (int i = 0; i < map->getEnemies().size(); i++) {
                            if (!map->getEnemies().at(i)->isDead()) {
                                playerBattle->attack(*(map->getEnemies()[i]));
                            }
                        }
                    }
                    // do something if player moved
                    if (moved) {
                        // TODO: make enemies drawn to player???
                        for (int i = 0; i < map->getEnemies().size(); i++) {
                            if (map->getEnemies().at(i)->isInBattle()) {
                                // if enemy not in range of player's attacks
                                if (!player->getAttackRangeCircle()->intersects(map->getEnemies().at(i)->getCircle())) {
                                    map->getEnemies().at(i)->setIsInBattle(false);
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
                // TODO: make enemies which are in battle to move towards player. otherwise, move randomly. if outside of wander circle, go back there and move randomly there
                if (!map->getEnemies().at(i)->isDead() && map->getEnemies().at(i)->canMove() && !map->getEnemies().at(i)->isInBattle()) {
                    // set enemy if not already set
                    if (enemiesMovement->getEntity() != map->getEnemies().at(i)) enemiesMovement->setEntity(*(map->getEnemies().at(i)));
                    // if route was calculated already, execute it
                    if (enemiesMovement->getEntity()->numOfMovesAvailable() > 0) enemiesMovement->moveBasedOnPoint(enemiesMovement->getEntity()->popMove());
                    else enemiesMovement->moveTowardsEntity(player, graph);
                    // choose random direction if not path to player exists
//                        int randomDirection = ((int) random()) % 4;
//                        enemiesMovement->moveRandomly(randomDirection);
                }
            }
            update();
            // render only when playing
            render();
            // resetting moved for enemies movement
            if (moved) moved = false;
        }
    }

    // deleting all maps from world map
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            delete worldMap[i][j];
        }
        delete[] worldMap[i];
    }

    delete playerMovement;
    delete enemiesMovement;
    delete playerBattle;

    disposeInstance();
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

void Game::update() {
    // updating player state
    player->update();
    // updating current map states
    GameMap *map = getCurrentGameMap();
    map->update();
}

void Game::initWorldMap() {
    // TODO: declare all maps here with unreachable areas and exit/enter points
    GameMap *map = worldMap[currentGameMapRow][currentGameMapRow];
    // setting exit and enter points
    map->setTopExit(SCREEN_WIDTH/2, SCREEN_WIDTH/2 + 2*TILE_SIZE);
    map->setTopEnterMinX(SCREEN_WIDTH/2);
    map->setTopEnterMaxX(SCREEN_WIDTH/2 + 2*TILE_SIZE);
    // adding unreachable areas and landscapes
//    auto *unreachableTree0 = new LandscapeEntity(LandscapeType::TREE, 3*(SCREEN_WIDTH/16), SCREEN_HEIGHT/6);
    auto *unreachableTree0 = new LandscapeEntity(LandscapeType::TREE, points[SCREEN_HEIGHT/6][3*SCREEN_WIDTH/16]);
//    auto *unreachableTree1 = new LandscapeEntity(LandscapeType::TREE, 9*(SCREEN_WIDTH/16), 1.7*SCREEN_HEIGHT/3);
    auto *unreachableTree1 = new LandscapeEntity(LandscapeType::TREE, points[(int)1.7*SCREEN_HEIGHT/3][9*SCREEN_WIDTH/16]);
    map->addLandscape(unreachableTree0);
    map->addLandscape(unreachableTree1);
    
    GameMap* mapTop = worldMap[currentGameMapRow - 1][currentGameMapCol];
    // setting exit and enter points
    mapTop->setBottomEnterMinX(SCREEN_WIDTH/2);
    mapTop->setBottomEnterMaxX(SCREEN_WIDTH/2 + 2*TILE_SIZE);
    mapTop->setBottomExit(SCREEN_WIDTH/2, SCREEN_WIDTH/2 + 2*TILE_SIZE);
    // adding unreachable areas and landscapes
//    auto *unreachableTree2 = new LandscapeEntity(LandscapeType::TREE, 6.5*SCREEN_WIDTH/8, 5*(SCREEN_HEIGHT/24));
    auto *unreachableTree2 = new LandscapeEntity(LandscapeType::TREE, points[5*SCREEN_HEIGHT/24][(int)6.5*SCREEN_WIDTH/8]);
//    auto *unreachableTree3 = new LandscapeEntity(LandscapeType::TREE, 3*(SCREEN_WIDTH/16), 7*(SCREEN_HEIGHT/12));
    auto *unreachableTree3 = new LandscapeEntity(LandscapeType::TREE, points[7*SCREEN_HEIGHT/12][3*SCREEN_WIDTH/16]);
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

Player *Game::getPlayer() {
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
    // graph->clear();
    // initGraphVertices();
    // initGraphEdges();
}
