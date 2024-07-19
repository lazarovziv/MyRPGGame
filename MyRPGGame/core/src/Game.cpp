#include "../include/Game.hpp"

Game* Game::instance = nullptr;

Game* Game::getInstance() {
    if (instance == nullptr) {
        std::cout << "Initializing game..." << std::endl;
        instance = new Game("MyRPGGame");
    }
    return instance;
}

void Game::disposeInstance() {
    delete instance;
    instance = nullptr;
}

Game::~Game() {
    delete title;
}

Game::Game(const char* str) {
    title = str;
    sf::VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
    std::string s(title);
    window = std::make_unique<sf::RenderWindow>(videoMode, s);

    window->setVerticalSyncEnabled(false);
//    window->setFramerateLimit(Constants::FPS);
    window->setFramerateLimit(0);

    cameraView = std::make_unique<sf::View>(sf::Vector2f(0, 0), sf::Vector2f(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));

    menuRepository = std::make_unique<MenuRepository>();
    initMenus();
    menuRepository->setMenu(currentMenu); // first menu is the game menu
    
    state = Constants::GameState::PLAYING;

    // init world map
    for (int r = 0; r < Constants::NUM_ROWS; r++) {
        std::vector<std::shared_ptr<GameMap>> rowVector;
        for (int c = 0; c < Constants::NUM_COLS; c++) {
            // cant go down but can go right, left and up
            if (r == Constants::NUM_ROWS - 1) {
                rowVector.push_back(std::make_shared<GameMap>(r, c, true, false, true, true));
                continue;
            }
            // cant go up but can go down, right, left
            if (r == 0) {
                rowVector.push_back(std::make_shared<GameMap>(r, c, false, true, true, true));
                continue;
            }
            // cant go right but can go down, up and left
            if (c == Constants::NUM_COLS - 1) {
                rowVector.push_back(std::make_shared<GameMap>(r, c, true, true, false, true));
                continue;
            }
            // cant go left but can go right, up and down
            if (c == 0) {
                rowVector.push_back(std::make_shared<GameMap>(r, c, true, true, true, false));
                continue;
            }
            rowVector.push_back(std::make_shared<GameMap>(r, c, true, true, true, true));
        }
        worldMap.push_back(rowVector);
    }

    currentGameMapRow = 1;
    currentGameMapCol = 1;
    
    this->player = std::make_shared<Player>(PlayerType::KNIGHT,
                                            physics::Vector{ (real) Constants::SCREEN_HEIGHT/2,
                                                             (real) Constants::SCREEN_WIDTH/2 });
    
    initWorldMap();
    changeCurrentMap(currentGameMapRow, currentGameMapCol);

    fpsFont.loadFromFile(Constants::GRAPHICS_BASE_PATH + "fonts/arial.ttf");
    fpsText.setFillColor(sf::Color::Blue);
    dtText.setFillColor(sf::Color::Blue);
    fpsText.setFont(fpsFont);
    dtText.setFont(fpsFont);
}

void Game::initEntities() {
    // initialize player's systems
     std::unique_ptr<GameEntityMovement> playerMovement = std::make_unique<GameEntityMovement>(
         player.get(), true, std::move(getCurrentGameMap()));
     std::unique_ptr<GameEntityBattle> playerBattle = std::make_unique<GameEntityBattle>(
         player.get()
     );
     std::unique_ptr<GameEntityMovement> enemiesMovement = std::make_unique<GameEntityMovement>(
             nullptr, false, std::move(getCurrentGameMap()));
    std::unique_ptr<GameEntityBattle> enemiesBattle = std::make_unique<GameEntityBattle>(
            nullptr
    );
//    auto *playerMovement = new GameEntityMovement(player.get(), true, std::move(getCurrentGameMap()));
//    auto *playerBattle = new GameEntityBattle(player.get());
//    auto *enemiesMovement = new GameEntityMovement(nullptr, false, std::move(getCurrentGameMap()));
//    auto *enemiesBattle = new GameEntityBattle(nullptr);

    playerRepository = std::make_unique<PlayerRepository>(player,
                                                          std::move(playerMovement),
                                                          std::move(playerBattle),
                                                          getCurrentGameMap());
    
    enemiesRepository = std::make_unique<EnemyRepository>(std::move(enemiesMovement),
                                                          std::move(enemiesBattle),
                                                          getCurrentGameMap());
}

void Game::initMenus() {
    std::vector<std::string> gameMenuItemsStrings = { "Start", "Settings", "Exit" };
    std::vector<std::string> mainMenuItemsStrings = { "Resume", "Inventory", "Settings", "Exit" };
    std::vector<std::string> characterCreationItemsStrings = { "Change Body", "Change Torso" };

    gameMenu = std::make_unique<Menu>(gameMenuItemsStrings, true);
    mainMenu = std::make_unique<Menu>(mainMenuItemsStrings, false);
    characterCreationMenu = std::make_unique<Menu>(characterCreationItemsStrings, false);

    gameMenu->addSubMenu(mainMenu.get(), 0);
    mainMenu->addSubMenu(characterCreationMenu.get(), 1);
    // gameMenu->addSubMenu(settingsMenu);
    currentMenu = gameMenu.get(); // initial menu is gameMenu
}

void Game::start() {
    std::cout << "Press Enter to start" << std::endl;
    std::cout << "Press I or Esc to enter menu" << std::endl;
    std::cout << "Press X near an enemy to attack" << std::endl;

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
    real dt, multiplier = (real) 12;

    std::map<sf::Keyboard::Key, bool> keysPressedMap;
    sf::Keyboard::Key upKey = sf::Keyboard::Key::W;
    sf::Keyboard::Key downKey = sf::Keyboard::Key::S;
    sf::Keyboard::Key rightKey = sf::Keyboard::Key::D;
    sf::Keyboard::Key leftKey = sf::Keyboard::Key::A;
    sf::Keyboard::Key runKey = sf::Keyboard::Key::LShift;
    sf::Keyboard::Key attackKey = sf::Keyboard::LControl;
    sf::Keyboard::Key jumpKey = sf::Keyboard::Space;
    sf::Keyboard::Key inventoryKey = sf::Keyboard::Key::I;
    sf::Keyboard::Key pauseKey = sf::Keyboard::Key::Escape;

    keysPressedMap[upKey] = false;
    keysPressedMap[downKey] = false;
    keysPressedMap[rightKey] = false;
    keysPressedMap[leftKey] = false;
    keysPressedMap[runKey] = false;
    keysPressedMap[attackKey] = false;
    keysPressedMap[jumpKey] = false;
    keysPressedMap[inventoryKey] = false;
    keysPressedMap[pauseKey] = false;

    EntityMovementState playerMovementState = EntityMovementState::WALK;

    physics::Vector directionVector = physics::Vector::ZERO;

    while (running) {
        dt = clock.restart().asSeconds();
        fpsText.setString("FPS: " + std::to_string(1/dt));
        dtText.setString("DT: " + std::to_string(dt));
        dt *= multiplier;
        // setting direction to 0
        directionVector.resetCoordinates();

        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                state = Constants::GameState::EXITING;
                // TODO: add save game and exit message confirmation
                running = false;
            }
        } // end poll event while loop

        canMove = state == Constants::GameState::PLAYING;
        eventKeyCode = event.key.code;

        if (state == Constants::GameState::PLAYING) {
            // moving input
            if (sf::Keyboard::isKeyPressed(upKey) && canMove) {
                keysPressedMap[upKey] = true;
                keysPressedMap[runKey] = sf::Keyboard::isKeyPressed(runKey) && canMove;
                // pressing the H key will trigger running state
                playerMovementState = keysPressedMap[runKey] ? EntityMovementState::RUN : EntityMovementState::WALK;
                directionVector += physics::Vector::UP_DIRECTION;
            }
            if (sf::Keyboard::isKeyPressed(downKey) && canMove) {
                keysPressedMap[downKey] = true;
                // pressing the H key will trigger running state
                keysPressedMap[runKey] = sf::Keyboard::isKeyPressed(runKey) && canMove;
                playerMovementState = keysPressedMap[runKey] ? EntityMovementState::RUN : EntityMovementState::WALK;
                directionVector += physics::Vector::DOWN_DIRECTION;
            }
            if (sf::Keyboard::isKeyPressed(rightKey) && canMove) {
                keysPressedMap[rightKey] = true;
                // pressing the H key will trigger running state
                keysPressedMap[runKey] = sf::Keyboard::isKeyPressed(runKey) && canMove;
                playerMovementState = keysPressedMap[runKey] ? EntityMovementState::RUN : EntityMovementState::WALK;
                directionVector += physics::Vector::RIGHT_DIRECTION;
            }
            if (sf::Keyboard::isKeyPressed(leftKey) && canMove) {
                keysPressedMap[leftKey] = true;
                // pressing the H key will trigger running state
                keysPressedMap[runKey] = sf::Keyboard::isKeyPressed(runKey) && canMove;
                playerMovementState = keysPressedMap[runKey] ? EntityMovementState::RUN : EntityMovementState::WALK;
                directionVector += physics::Vector::LEFT_DIRECTION;
            }

            playerRepository->move(directionVector, keysPressedMap[runKey], dt);

            enemiesRepository->setGameMap(getCurrentGameMap());

            // pressing x for attacking
            // TODO: use player repository instead of player
            if (player->isAttacking() || sf::Keyboard::isKeyPressed(attackKey)) {
                attacked = playerRepository->attack(EntityMovementState::COMBAT_SLASH_ONE_HANDED, dt);
                keysPressedMap[attackKey] = true;
            }

            // pressing space for jumping
            if (player->isJumping() || sf::Keyboard::isKeyPressed(jumpKey)) {
                playerRepository->jump(directionVector, dt);
                keysPressedMap[jumpKey] = true;
            }

            // pressing escape sends to menu
            if (sf::Keyboard::isKeyPressed(pauseKey)) {
                keysPressedMap[pauseKey] = true;
                changeState(Constants::GameState::IN_MENU);
                std::cout << "In Menu (Press Space to choose)" << std::endl;
                canMove = false;
                // pressing I sends to inventory menu (to be implemented)
            } else if (eventKeyCode == inventoryKey) {
                changeState(Constants::GameState::IN_MENU);
                // currentMenu = inventoryMenu;
                std::cout << "Inventory Menu" << std::endl;
                canMove = false;
            }
            // TODO: add key for going straight to character creation
        } else if (state == Constants::GameState::IN_MENU) {
            if (sf::Keyboard::isKeyPressed(upKey)) {
                menuRepository->moveUp();
            } else if (sf::Keyboard::isKeyPressed(downKey)) {
                menuRepository->moveDown();
                // user chose an item menu
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                // set current menu to be the relevant one based on currentMenuItemIdx
                if (menuRepository->choseSubMenuItem()) {
                    menuRepository->updateSubMenu(currentMenu, &state);
                    //updateMenu(currentMenu, &running, &canMove, menuRepository->getMenuType());
                } else {
                    // execute the menu item functionality (cant overflow as it's defined only for non submenu indexes in the class)
                    switch (menuRepository->execute(&state)) {
                        case 0: {
                            changeState(Constants::GameState::PLAYING);
                            std::cout << "Returning to game..." << std::endl;
                            break;
                        }
                        case 1: {
                            break;
                        }
                        case 2: {
                            changeState(Constants::GameState::EXITING);
                            exitGame(&running);
                            break;
                        }
                    }
                }
                // going back one menu
            } else if (sf::Keyboard::isKeyPressed(pauseKey)) {
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
            enemiesRepository->move(player->getPosition(), false, dt);
            // update physics
            for (int i = 0; i < Constants::UPDATE_ITERATIONS; i++) {
                getCurrentGameMap()->resolveCollisions(dt);
            }
            // update all entities' states when playing
            update(dt);
        }

        for (auto &key : keysPressedMap) {
            keysPressedMap[key.first] = false;
        }
        // render playing or main menu or game menu
        render();
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

std::shared_ptr<GameMap> Game::getCurrentGameMap() {
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
        std::shared_ptr<GameMap> map = getCurrentGameMap();
        // draw background
        window->draw(*(map->getBackgroundSprite()));
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

        // drawing unreachable areas
        for (int i = 0; i < map->getLandscapes().size(); i++) {
            window->draw(*(map->getLandscapes()[i]->getSprite()));
        }
    }
    // display all drawn sprites
    window->display();
}

void Game::renderMenu(Menu *menu) {
    menu->render(player->getPosition().x,
                 player->getPosition().y, window.get());
}

void Game::update(const real dt) {
    // updating current map states
    getCurrentGameMap()->update(dt);
    // updating player state
    // playerRepository->update(dt);

    cameraView->setCenter(player->getRenderPosition());
    window->setView(*cameraView);
    // setting at the left hand corner in according to the player's position
    fpsText.setPosition(player->getRenderPosition().x - SCREEN_WIDTH/2, player->getRenderPosition().y - SCREEN_HEIGHT/2);
    dtText.setPosition(player->getRenderPosition().x - SCREEN_WIDTH/2, player->getRenderPosition().y - SCREEN_HEIGHT/2 + 2*fpsText.getGlobalBounds().height);
}

void Game::updateMenu(Menu *menu, bool *run, bool *move) {
    // executing command
    menuRepository->execute(&state);
    // resetting idx
    menuRepository->resetMenuItemIdx();
}

void Game::initWorldMap() {
    // TODO: declare all maps here with unreachable areas and exit/enter points
    std::shared_ptr<GameMap> startMap = worldMap[currentGameMapRow][currentGameMapRow];
    // TODO: AUTOMATE THIS SECTION
    physics::Vector bottomLeftOffset = physics::Vector{-1, 1};
    physics::Vector bottomRightOffset = physics::Vector{1, 1};
    physics::Vector topLeftOffset = physics::Vector{-1, -1};
    physics::Vector topRightOffset = physics::Vector{1, -1};
    real edgeLength = TILE_SIZE;
    // top exit circle
    auto *startMapTopExitCircle = new Circle(TILE_SIZE / 2, FULL_SCREEN_WIDTH / 2 + TILE_SIZE,
                                             TILE_SIZE / 2);
    startMap->setTopExitCircle(startMapTopExitCircle);
    /*
    physics::Vector unreachableTree0Center = physics::Vector{FULL_SCREEN_HEIGHT/6, 3*FULL_SCREEN_WIDTH/16};
    std::vector<physics::Vector> unreachableTree0Vertices = {
            unreachableTree0Center + bottomLeftOffset * edgeLength, // bottom left
            unreachableTree0Center + bottomRightOffset * edgeLength, // bottom right
            unreachableTree0Center + topLeftOffset * edgeLength, // top left
            unreachableTree0Center + topRightOffset * edgeLength
    };
    // adding unreachable areas and landscapes
    auto *unreachableTree0 = new LandscapeEntity(LandscapeType::TREE,
                                                 unreachableTree0Center,
                                                 unreachableTree0Vertices);
    physics::Vector unreachableTree1Center = physics::Vector{1.7*FULL_SCREEN_HEIGHT/3, 9*FULL_SCREEN_WIDTH/16};
    std::vector<physics::Vector> unreachableTree1Vertices = {
            unreachableTree1Center + bottomLeftOffset * edgeLength, // bottom left
            unreachableTree1Center + bottomRightOffset * edgeLength, // bottom right
            unreachableTree1Center + topLeftOffset * edgeLength, // top left
            unreachableTree1Center + topRightOffset * edgeLength
    };
    auto *unreachableTree1 = new LandscapeEntity(LandscapeType::TREE,
                                                 unreachableTree1Center,
                                                 unreachableTree1Vertices);
    physics::Vector unreachableTree22Center = physics::Vector{1.7*FULL_SCREEN_HEIGHT/3, 7*FULL_SCREEN_WIDTH/16};
    std::vector<physics::Vector> unreachableTree22Vertices = {
            unreachableTree22Center + bottomLeftOffset * edgeLength, // bottom left
            unreachableTree22Center + bottomRightOffset * edgeLength, // bottom right
            unreachableTree22Center + topLeftOffset * edgeLength, // top left
            unreachableTree22Center + topRightOffset * edgeLength
    };
    auto *unreachableTree22 = new LandscapeEntity(LandscapeType::TREE,
                                                  unreachableTree22Center,
                                                  unreachableTree22Vertices);
    physics::Vector unreachableTree33Center = physics::Vector{1.7*FULL_SCREEN_HEIGHT/3, 5*FULL_SCREEN_WIDTH/16};
    std::vector<physics::Vector> unreachableTree33Vertices = {
            unreachableTree33Center + bottomLeftOffset * edgeLength, // bottom left
            unreachableTree33Center + bottomRightOffset * edgeLength, // bottom right
            unreachableTree33Center + topLeftOffset * edgeLength, // top left
            unreachableTree33Center + topRightOffset * edgeLength
    };
    auto *unreachableTree33 = new LandscapeEntity(LandscapeType::TREE,
                                                  unreachableTree33Center,
                                                  unreachableTree33Vertices);

    */
    physics::Vector house0Center = physics::Vector{535, 95};
    std::vector<physics::Vector> house0Vertices {
            house0Center + (bottomLeftOffset * edgeLength), // bottom left
            house0Center + (bottomRightOffset * edgeLength), // bottom right
            house0Center + (topRightOffset * edgeLength), // top right
            house0Center + (topLeftOffset * edgeLength) // top left
    };
    auto *unreachableHouse0 = new LandscapeEntity(LandscapeType::TREE,
                                                 house0Center,
                                                 house0Vertices);

    startMap->addLandscape(unreachableHouse0);

    std::shared_ptr<GameMap> topMap = worldMap[currentGameMapRow - 1][currentGameMapCol];
    // bottom exit circle
    auto *topMapBottomExitCircle = new Circle(
    FULL_SCREEN_HEIGHT-TILE_SIZE/2, FULL_SCREEN_WIDTH/2 + TILE_SIZE, TILE_SIZE/2);
    topMap->setBottomExitCircle(topMapBottomExitCircle);
    // adding unreachable areas and landscapes
    physics::Vector unreachableTree2Center = physics::Vector{5*FULL_SCREEN_HEIGHT/24, 6.5*FULL_SCREEN_WIDTH/8};
    std::vector<physics::Vector> unreachableTree2Vertices = {
            unreachableTree2Center + bottomLeftOffset * edgeLength, // bottom left
            unreachableTree2Center + bottomRightOffset * edgeLength, // bottom right
            unreachableTree2Center + topLeftOffset * edgeLength, // top left
            unreachableTree2Center + topRightOffset * edgeLength
    };
    auto *unreachableTree2 = new LandscapeEntity(LandscapeType::TREE,
                                                 unreachableTree2Center,
                                                 unreachableTree2Vertices);
    physics::Vector unreachableTree3Center = physics::Vector{7*FULL_SCREEN_HEIGHT/12, 3*FULL_SCREEN_WIDTH/16};
    std::vector<physics::Vector> unreachableTree3Vertices = {
            unreachableTree3Center + bottomLeftOffset * edgeLength, // bottom left
            unreachableTree3Center + bottomRightOffset * edgeLength, // bottom right
            unreachableTree3Center + topLeftOffset * edgeLength, // top left
            unreachableTree3Center + topRightOffset * edgeLength
    };
    auto *unreachableTree3 = new LandscapeEntity(LandscapeType::TREE,
                                                 unreachableTree3Center,
                                                 unreachableTree3Vertices);
    topMap->addLandscape(unreachableTree2);
    topMap->addLandscape(unreachableTree3);

    // deallocate memory if needed
}

void Game::setCurrentWorldMapRow(const int row) {
    currentGameMapRow = row;
}

void Game::setCurrentWorldMapCol(const int col) {
    currentGameMapCol = col;
}

void Game::changeCurrentMap(const int row, const int col) {
    if (row < 0 || row >= Constants::NUM_ROWS) return;
    if (col < 0 || col >= Constants::NUM_COLS) return;
    // abandoning previous map and setting its player attribute to null
    worldMap[currentGameMapRow][currentGameMapCol]->removePlayer();
    // change current map
    setCurrentWorldMapRow(row);
    setCurrentWorldMapCol(col);
    // initialize map (or reinitialize?)
    worldMap[currentGameMapRow][currentGameMapCol]->init();
    // define the new game map for the entities
    if (playerRepository != nullptr) playerRepository->setGameMap(std::move(getCurrentGameMap()));
    if (enemiesRepository != nullptr) enemiesRepository->setGameMap(std::move(getCurrentGameMap()));
}
