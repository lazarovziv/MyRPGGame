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

    cameraView = new View(Vector2f(0, 0),
                          Vector2f(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));

    std::vector<std::string> mainMenuItemsStrings = { "Start", "Settings", "Exit" };
    std::vector<std::string> gameMenuItemsStrings = { "Resume", "Settings", "Exit" };
    mainMenu = new MainMenu(mainMenuItemsStrings);
    gameMenu = new Menu(gameMenuItemsStrings);

    state = GameState::PAUSED;

    // init all possible points
    points = new Point **[Constants::FULL_SCREEN_HEIGHT];
    for (int row = 0; row < Constants::FULL_SCREEN_HEIGHT; row++) {
        points[row] = new Point *[Constants::FULL_SCREEN_WIDTH];
        for (int col = 0; col < Constants::FULL_SCREEN_WIDTH; col++) {
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
    
    this->player = new Player(PlayerType::KNIGHT,
                              points[Constants::SCREEN_HEIGHT/2][Constants::SCREEN_WIDTH/2]);
    
    initWorldMap();
    changeCurrentMap(currentGameMapRow, currentGameMapCol);
    // init first map
//    getCurrentGameMap()->init();
}

void Game::start() {
    cout << "Press Enter to start" << endl;
    cout << "Press I or Esc to enter menu" << endl;
    cout << "Press X near an enemy to attack" << endl;

    // initialize player's systems
    auto *playerMovement = new GameEntityMovement(player, true, getCurrentGameMap(), points);
    auto *playerBattle = new GameEntityBattle(player);
    auto *enemiesMovement = new GameEntityMovement(nullptr, false, getCurrentGameMap(), points);
    auto *enemiesBattle = new GameEntityBattle(nullptr);

    playerRepository = new PlayerRepository(player, playerMovement,
                                            playerBattle, getCurrentGameMap());
    enemiesRepository = new EnemyRepository(enemiesMovement, enemiesBattle,
                                            player, getCurrentGameMap());

    bool canMove = false;
    bool running = window->isOpen();

    Event event;

    int eventKeyCode;
    bool moved = false;
    Constants::MoveSuccessValues moveSuccessValue;

    // game loop
    while (running) {
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                state = GameState::EXITING;
                // add save game and exit message confirmation
                running = false;
            }

            if (event.type == Event::KeyPressed) {
                eventKeyCode = event.key.code;

                if (state == GameState::PLAYING) {
                    // moving with the arrows
                    if (eventKeyCode == Keyboard::Up && canMove) {
                        moveSuccessValue = playerRepository->move(MoveDirection::UP);
                        moved = moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    } else if (eventKeyCode == Keyboard::Down && canMove) {
                        moveSuccessValue = playerRepository->move(MoveDirection::DOWN);
                        moved = moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    } else if (eventKeyCode == Keyboard::Right && canMove) {
                        moveSuccessValue = playerRepository->move(MoveDirection::RIGHT);
                        moved = moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    } else if (eventKeyCode == Keyboard::Left && canMove) {
                        moveSuccessValue = playerRepository->move(MoveDirection::LEFT);
                        moved = moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    }

                    // switching world map according to value returned from move function
                    switch (moveSuccessValue) {
                        case Constants::MoveSuccessValues::CHANGE_UP:
                            changeCurrentMap(currentGameMapRow-1, currentGameMapCol);
                            enemiesMovement->setCurrentMap(getCurrentGameMap());
                            break;
                        case Constants::MoveSuccessValues::CHANGE_DOWN:
                            changeCurrentMap(currentGameMapRow+1, currentGameMapCol);
                            enemiesMovement->setCurrentMap(getCurrentGameMap());
                            break;
                        case Constants::MoveSuccessValues::CHANGE_RIGHT:
                            changeCurrentMap(currentGameMapRow, currentGameMapCol+1);
                            enemiesMovement->setCurrentMap(getCurrentGameMap());
                            break;
                        case Constants::MoveSuccessValues::CHANGE_LEFT:
                            changeCurrentMap(currentGameMapRow, currentGameMapCol-1);
                            enemiesMovement->setCurrentMap(getCurrentGameMap());
                            break;
                        default:
                            break;
                    }

                    // pressing I sends to menu (not implemented menu yet)
                    if (eventKeyCode == Keyboard::I || eventKeyCode == Keyboard::Escape) {
                        changeState(GameState::IN_MENU);
                        cout << "In Menu (Press Space to choose)" << endl;
                        canMove = false;
                    }
                    // pressing x for attacking
                    if (eventKeyCode == Keyboard::X) {
                        playerRepository->attack();
                    }
                } else if (state == GameState::IN_MENU) {
                    // moving with the arrows
                    if (eventKeyCode == Keyboard::Up) {
                        gameMenu->moveUp();
                    } else if (eventKeyCode == Keyboard::Down) {
                        gameMenu->moveDown();
                    } else if (eventKeyCode == Keyboard::Space) {
                        updateMenu(gameMenu, &running, &canMove);
                    }
                } else if (state == GameState::PAUSED) {
                    // moving with the arrows
                    if (eventKeyCode == Keyboard::Up) {
                        mainMenu->moveUp();
                    } else if (eventKeyCode == Keyboard::Down) {
                        mainMenu->moveDown();
                    } else if (eventKeyCode == Keyboard::Space) {
                        updateMenu(mainMenu, &running, &canMove);
                    }
                }
            } // end key pressed
        } // end poll event while loop

        if (state == GameState::PLAYING) {
            // make enemies move
            enemiesRepository->move();
            // update all entities' states when playing
            update(moveSuccessValue);
            // resetting moved for enemies movement. moved = false iff moveSuccessValue = FAILURE
            if (moved) {
                moved = false;
                moveSuccessValue = Constants::MoveSuccessValues::FAILURE;
            } else moveSuccessValue = Constants::MoveSuccessValues::NOT_MOVED;
        }
        // render playing or main menu or game menu
        render();
    }

    // deleting all maps from world map
    for (int i = 0; i < Constants::NUM_ROWS; i++) {
        for (int j = 0; j < Constants::NUM_COLS; j++) {
            delete worldMap[i][j];
        }
        delete[] worldMap[i];
    }

    delete playerMovement;
    delete enemiesMovement;
    delete playerBattle;
}

void Game::changeState(GameState gameState) {
    this->state = gameState;
    // TODO: change running and canMove local variables in start method
}

GameMap* Game::getCurrentGameMap() {
    return worldMap[currentGameMapRow][currentGameMapCol];
}

void Game::render() {
    // clear window
    window->clear();
    // draw based on game state
    if (state == GameState::PAUSED) {
        renderMenu(mainMenu);
    } else if (state == GameState::IN_MENU) {
        renderMenu(gameMenu);
    } else if (state == GameState::PLAYING) {
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
    }
    // display all drawn sprites
    window->display();
}

void Game::renderMenu(Menu *menu) {
    menu->render(player->getCircle()->getCenter()->getX(),
                 player->getCircle()->getCenter()->getY(), window);
}

void Game::update(Constants::MoveSuccessValues playerMoveSuccessValue) {
    // updating player state
    playerRepository->update(points, playerMoveSuccessValue);
    // updating current map states
    enemiesRepository->update();
    cameraView->setCenter((Vector2f) player->getPosition());
    window->setView(*cameraView);
}

void Game::updateMenu(Menu *menu, bool *run, bool *move) {
    // executing command
    switch (menu->execute()) {
        case MenuActions::ACTION_RESUME: {
            state = GameState::PLAYING;
            cout << "Resuming..." << endl;
            *move = true;
            break;
        }
        case MenuActions::ACTION_SETTINGS: {
            // TODO: create settings menu
            cout << "Settings Menu" << endl;
            break;
        }
        case MenuActions::ACTION_EXIT: {
            state = GameState::EXITING;
            cout << "Exiting Game..." << endl;
            *run = false;
            break;
        }
    }
    // resetting idx
    menu->resetMenuItemIdx();
}

void Game::initWorldMap() {
    // TODO: declare all maps here with unreachable areas and exit/enter points
    GameMap *startMap = worldMap[currentGameMapRow][currentGameMapRow];
    // top exit circle
    auto *startMapTopExitCircle = new Circle(points[TILE_SIZE / 2][FULL_SCREEN_WIDTH / 2 + TILE_SIZE],
                                             TILE_SIZE / 2);
    startMap->setTopExitCircle(startMapTopExitCircle);
    // adding unreachable areas and landscapes
    auto *unreachableTree0 = new LandscapeEntity(LandscapeType::TREE, points[FULL_SCREEN_HEIGHT/6][3*FULL_SCREEN_WIDTH/16]);
    auto *unreachableTree1 = new LandscapeEntity(LandscapeType::TREE, points[(int)1.7*FULL_SCREEN_HEIGHT/3][9*FULL_SCREEN_WIDTH/16]);
    auto *unreachableTree22 = new LandscapeEntity(LandscapeType::TREE, points[(int)1.7*FULL_SCREEN_HEIGHT/3][7*FULL_SCREEN_WIDTH/16]);
    auto *unreachableTree33 = new LandscapeEntity(LandscapeType::TREE, points[(int)1.7*FULL_SCREEN_HEIGHT/3][5*FULL_SCREEN_WIDTH/16]);
    startMap->addLandscape(unreachableTree0);
    startMap->addLandscape(unreachableTree1);
    startMap->addLandscape(unreachableTree22);
    startMap->addLandscape(unreachableTree33);

    GameMap* topMap = worldMap[currentGameMapRow - 1][currentGameMapCol];
    // bottom exit circle
    auto *topMapBottomExitCircle = new Circle(
            points[FULL_SCREEN_HEIGHT-TILE_SIZE/2][FULL_SCREEN_WIDTH/2 + TILE_SIZE],TILE_SIZE/2);
    topMap->setBottomExitCircle(topMapBottomExitCircle);
    // adding unreachable areas and landscapes
    auto *unreachableTree2 = new LandscapeEntity(LandscapeType::TREE,
                                                 points[5*FULL_SCREEN_HEIGHT/24][(int)6.5*FULL_SCREEN_WIDTH/8]);
    auto *unreachableTree3 = new LandscapeEntity(LandscapeType::TREE,
                                                 points[7*FULL_SCREEN_HEIGHT/12][3*FULL_SCREEN_WIDTH/16]);
    topMap->addLandscape(unreachableTree2);
    topMap->addLandscape(unreachableTree3);


    // deallocate memory if needed
}

void Game::setCurrentWorldMapRow(int row) {
    currentGameMapRow = row;
}

void Game::setCurrentWorldMapCol(int col) {
    currentGameMapCol = col;
}

void Game::changeCurrentMap(int row, int col) {
    if (row < 0 || row >= Constants::NUM_ROWS) return;
    if (col < 0 || col >= Constants::NUM_COLS) return;
    // abandoning previous map and setting its player attribute to null
    worldMap[currentGameMapRow][currentGameMapCol]->setPlayer(nullptr);
    // change current map
    setCurrentWorldMapRow(row);
    setCurrentWorldMapCol(col);
    // initialize map
    worldMap[currentGameMapRow][currentGameMapCol]->init();
    if (playerRepository != nullptr) playerRepository->setGameMap(getCurrentGameMap());
    if (enemiesRepository != nullptr) enemiesRepository->setGameMap(getCurrentGameMap());
}
