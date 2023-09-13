#include "../include/Game.hpp"

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

    delete worldMap;
    delete title;
}

Game::Game(const char* str) {
    title = str;
    VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
    std::string s(title);
    window = make_unique<RenderWindow>(videoMode, s);

    window->setVerticalSyncEnabled(false);
//    window->setFramerateLimit(Constants::FPS);
    window->setFramerateLimit(0);

    cameraView = make_unique<View>(Vector2f(0, 0), Vector2f(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));

    menuRepository = make_unique<MenuRepository>();
    initMenus();
    menuRepository->setMenu(currentMenu); // first menu is the game menu
    
    state = Constants::GameState::IN_MENU;

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
    
    this->player = make_unique<Player>(PlayerType::KNIGHT,
                              points[Constants::SCREEN_HEIGHT/2][Constants::SCREEN_WIDTH/2]);
    
    initWorldMap();
    changeCurrentMap(currentGameMapRow, currentGameMapCol);

    fpsFont.loadFromFile("../graphics/fonts/arial.ttf");
    fpsText.setFillColor(Color::Blue);
    dtText.setFillColor(Color::Blue);
    fpsText.setFont(fpsFont);
    dtText.setFont(fpsFont);
    // init first map
//    getCurrentGameMap()->init();
}

void Game::initEntities() {
    // initialize player's systems
    auto *playerMovement = new GameEntityMovement(player.get(), true, getCurrentGameMap(), points);
    auto *playerBattle = new GameEntityBattle(player.get());
    auto *enemiesMovement = new GameEntityMovement(nullptr, false, getCurrentGameMap(), points);
    auto *enemiesBattle = new GameEntityBattle(nullptr);

    playerRepository = make_unique<PlayerRepository>(player.get(), playerMovement,
                                            playerBattle, getCurrentGameMap());
    enemiesRepository = make_unique<EnemyRepository>(enemiesMovement, enemiesBattle,
                                            player.get(), getCurrentGameMap());
}

void Game::initMenus() {
    std::vector<std::string> gameMenuItemsStrings = { "Start", "Settings", "Exit" };
    std::vector<std::string> mainMenuItemsStrings = { "Resume", "Inventory", "Settings", "Exit" };
    std::vector<std::string> characterCreationItemsStrings = { "Change Body", "Change Torso" };

    gameMenu = make_unique<Menu>(gameMenuItemsStrings, true);
    mainMenu = make_unique<Menu>(mainMenuItemsStrings, false);
    characterCreationMenu = make_unique<Menu>(characterCreationItemsStrings, false);
    gameMenu->addSubMenu(mainMenu.get(), 0);
    mainMenu->addSubMenu(characterCreationMenu.get(), 1);
    // gameMenu->addSubMenu(settingsMenu);
    currentMenu = gameMenu.get(); // initial menu is gameMenu
}

void Game::start() {
    cout << "Press Enter to start" << endl;
    cout << "Press I or Esc to enter menu" << endl;
    cout << "Press X near an enemy to attack" << endl;

    initEntities();

    bool canMove = false;
    bool running = window->isOpen();

    sf::Event event;

    int eventKeyCode;
    bool moved = false;
    bool attacked = false;
    Constants::MoveSuccessValues moveSuccessValue;

    sf::Clock clock;
    // TODO: tweak multiplier value (12 seems to be close, maybe fps/10)
    float dt, multiplier = 12.f;

    std::map<Keyboard::Key, bool> keysPressedMap;
    keysPressedMap[Keyboard::Key::E] = false;
    keysPressedMap[Keyboard::Key::D] = false;
    keysPressedMap[Keyboard::Key::F] = false;
    keysPressedMap[Keyboard::Key::S] = false;
    keysPressedMap[Keyboard::Key::J] = false;
    keysPressedMap[Keyboard::Key::I] = false;
    keysPressedMap[Keyboard::Key::H] = false;
    keysPressedMap[Keyboard::Key::Escape] = false;

    EntityMovementState playerMovementState = EntityMovementState::WALK;

    while (running) {
        dt = clock.restart().asSeconds();
        fpsText.setString("FPS: " + to_string(1/dt));
        dtText.setString("DT: " + to_string(dt));
        dt *= multiplier;

        // TODO: delta time fucks things if it's not in the pollEvent loop. processing input is outside of the loop we need to multiply each position by dt
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                state = Constants::GameState::EXITING;
                // TODO: add save game and exit message confirmation
                running = false;
            }
        } // end poll event while loop

        canMove = state == Constants::GameState::PLAYING;
        eventKeyCode = event.key.code;

        if (state == Constants::GameState::PLAYING) {
            // moving input
            if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::E) && canMove) {
                keysPressedMap[Keyboard::E] = true;
                keysPressedMap[Keyboard::H] = Keyboard::isKeyPressed(Keyboard::H) && canMove;
                playerMovementState = keysPressedMap[Keyboard::H] ? EntityMovementState::RUN : EntityMovementState::WALK;
                // allowing diagonal movement
                if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::F)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::UP_RIGHT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::F] = true;
                } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::S)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::UP_LEFT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::S] = true;
                } else {
                    moveSuccessValue = playerRepository->move(MoveDirection::UP, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                }
            } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::D) && canMove) {
                keysPressedMap[Keyboard::D] = true;
                keysPressedMap[Keyboard::H] = Keyboard::isKeyPressed(Keyboard::H) && canMove;
                playerMovementState = keysPressedMap[Keyboard::H] ? EntityMovementState::RUN : EntityMovementState::WALK;
                // allowing diagonal movement
                if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::F)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::DOWN_RIGHT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::F] = true;
                } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::S)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::DOWN_LEFT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::S] = true;
                } else {
                    moveSuccessValue = playerRepository->move(MoveDirection::DOWN, playerMovementState, dt);
                    moved = moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                }
            } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::F) && canMove) {
                keysPressedMap[Keyboard::F] = true;
                keysPressedMap[Keyboard::H] = Keyboard::isKeyPressed(Keyboard::H) && canMove;
                playerMovementState = keysPressedMap[Keyboard::H] ? EntityMovementState::RUN : EntityMovementState::WALK;
                // allowing diagonal movement
                if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::E)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::UP_RIGHT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::E] = true;
                } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::D)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::DOWN_RIGHT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::D] = true;
                } else {
                    moveSuccessValue = playerRepository->move(MoveDirection::RIGHT, playerMovementState, dt);
                    moved = moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                }
            } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::S) && canMove) {
                keysPressedMap[Keyboard::S] = true;
                keysPressedMap[Keyboard::H] = Keyboard::isKeyPressed(Keyboard::H) && canMove;
                playerMovementState = keysPressedMap[Keyboard::H] ? EntityMovementState::RUN : EntityMovementState::WALK;
                // allowing diagonal movement
                if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::E)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::UP_LEFT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::E] = true;
                } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::D)) {
                    moveSuccessValue = playerRepository->move(MoveDirection::DOWN_LEFT, playerMovementState, dt);
                    moved = moved && moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                    keysPressedMap[Keyboard::D] = true;
                } else {
                    moveSuccessValue = playerRepository->move(MoveDirection::LEFT, playerMovementState, dt);
                    moved = moveSuccessValue != Constants::MoveSuccessValues::FAILURE;
                }
            }

            if (moved || keysPressedMap[Keyboard::E] || keysPressedMap[Keyboard::S] || keysPressedMap[Keyboard::D] || keysPressedMap[Keyboard::F]) {
                // switching world map according to value returned from move function
                switch (moveSuccessValue) {
                    case Constants::MoveSuccessValues::CHANGE_UP:
                        changeCurrentMap(currentGameMapRow-1, currentGameMapCol);
                        break;
                    case Constants::MoveSuccessValues::CHANGE_DOWN:
                        changeCurrentMap(currentGameMapRow+1, currentGameMapCol);
                        break;
                    case Constants::MoveSuccessValues::CHANGE_RIGHT:
                        changeCurrentMap(currentGameMapRow, currentGameMapCol+1);
                        break;
                    case Constants::MoveSuccessValues::CHANGE_LEFT:
                        changeCurrentMap(currentGameMapRow, currentGameMapCol-1);
                        break;
                    default: break;
                }
            } else if (!keysPressedMap[Keyboard::J] && player->canGoIdle()) {
                playerRepository->move(player->getMoveDirection(),
                                       EntityMovementState::IDLE,
                                       dt);
            }

            enemiesRepository->setGameMap(getCurrentGameMap());

            // pressing x for attacking
            if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::J)) {
                attacked = playerRepository->attack(dt);
                keysPressedMap[Keyboard::J] = true;
            }

            // pressing escape sends to menu
            if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::Escape)) {
                keysPressedMap[Keyboard::Escape] = true;
                changeState(Constants::GameState::IN_MENU);
                cout << "In Menu (Press Space to choose)" << endl;
                canMove = false;
                // pressing I sends to inventory menu (to be implemented)
            } else if (eventKeyCode == Keyboard::I) {
                changeState(Constants::GameState::IN_MENU);
                // currentMenu = inventoryMenu;
                cout << "Inventory Menu" << endl;
                canMove = false;
            }
            // TODO: add key for going straight to character creation
        } else if (state == Constants::GameState::IN_MENU) {
            if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::E)) {
                menuRepository->moveUp();
            } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::D)) {
                menuRepository->moveDown();
                // user chose an item menu
            } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::Space) || /*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::Enter)) {
                // set current menu to be the relevant one based on currentMenuItemIdx
                if (menuRepository->choseSubMenuItem()) {
                    menuRepository->updateSubMenu(currentMenu, &state);
                    //updateMenu(currentMenu, &running, &canMove, menuRepository->getMenuType());
                } else {
                    // execute the menu item functionality (cant overflow as it's defined only for non submenu indexes in the class)
                    switch (menuRepository->execute(&state)) {
                        case 0: {
                            changeState(Constants::GameState::PLAYING);
                            cout << "Returning to game..." << endl;
                            break;
                        }
                        case 1: {
                            break;
                        }
                        case 2: {
                            exitGame(&running);
                            break;
                        }
                    }
                }
                // going back one menu
            } else if (/*eventKeyCode == */Keyboard::isKeyPressed(Keyboard::Escape) && keysPressedMap[Keyboard::Escape]) {
                cout << "HEY" << endl;
                // if it's the game menu then we'll exit the game
                if (menuRepository->isGameMenu()) {
                    exitGame(&running);
                    break;
                }
                // not a game menu
                currentMenu = currentMenu->getParentMenu();
                menuRepository->setMenu(currentMenu);
            }
        }

        // TODO: add save game functionality
        if (state == Constants::GameState::EXITING) {
            exitGame(&running);
            break;
        }

        if (state == Constants::GameState::PLAYING) {
            // make enemies move
            enemiesRepository->move(dt);
            // update all entities' states when playing
            update(moveSuccessValue, dt);
            // resetting moved for enemies movement. moved = false iff moveSuccessValue = FAILURE
            if (moved) {
                moved = false;
                moveSuccessValue = Constants::MoveSuccessValues::FAILURE;
            } else {
                moveSuccessValue = Constants::MoveSuccessValues::NOT_MOVED;
            }
        }

        for (auto &key : keysPressedMap) {
            keysPressedMap[key.first] = false;
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
}

void Game::exitGame(bool *run) {
    // do all things before closing the game session (autosave or whatever)
    *run = false;
}

void Game::changeState(Constants::GameState gameState) {
    this->state = gameState;
    // TODO: change running and canMove local variables in start method
}

GameMap* Game::getCurrentGameMap() {
    return worldMap[currentGameMapRow][currentGameMapCol];
}

void Game::render() {
    // clear window
    window->clear();
    // TODO: handle case where state == IS_EXITING
    // draw based on game state
    if (state != Constants::GameState::PLAYING) {
        renderMenu(currentMenu);
    } else {
        GameMap *map = getCurrentGameMap();
        // draw background
        window->draw(*(map->getBackgroundSprite()));
        // drawing unreachable areas
        for (int i = 0; i < map->getLandscapes().size(); i++) {
            window->draw(*(map->getLandscapes()[i]->getSprite()));
        }
        // draw fps and dt
        window->draw(fpsText);
        window->draw(dtText);
        // drawing undead enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (!map->getEnemies()[i]->isDead()) {
                window->draw(*(map->getEnemies()[i]->getSprite()));
                // draw enemies' weapon
            }
        }
        window->draw(*(player->getSprite()));
        window->draw(*(player->getWeapon()->getSprite()));
    }
    // display all drawn sprites
    window->display();
}

void Game::renderMenu(Menu *menu) {
    menu->render(player->getCircle()->getCenter()->getX(),
                 player->getCircle()->getCenter()->getY(), window.get());
}

void Game::update(Constants::MoveSuccessValues playerMoveSuccessValue, float dt) {
    // updating player state
    playerRepository->update(points, playerMoveSuccessValue, dt);
    // updating current map states
    enemiesRepository->update(dt);
    cameraView->setCenter(player->getPosition());
    window->setView(*cameraView);
    // setting at the left hand corner in according to the player's position
    fpsText.setPosition(player->getPosition().x - SCREEN_WIDTH/2, player->getPosition().y - SCREEN_HEIGHT/2);
    dtText.setPosition(player->getPosition().x - SCREEN_WIDTH/2, player->getPosition().y - SCREEN_HEIGHT/2 + 2*fpsText.getGlobalBounds().height);
}

void Game::updateMenu(Menu *menu, bool *run, bool *move) {
    // executing command
    menuRepository->execute(&state);
    // resetting idx
    menuRepository->resetMenuItemIdx();
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
    // initialize map (or reinitialize?)
    worldMap[currentGameMapRow][currentGameMapCol]->init();
    // define the new game map for the entities
    if (playerRepository != nullptr) playerRepository->setGameMap(getCurrentGameMap());
    if (enemiesRepository != nullptr) enemiesRepository->setGameMap(getCurrentGameMap());
}
