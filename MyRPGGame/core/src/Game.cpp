#include "Game.hpp"

std::unique_ptr<Game> Game::instance = nullptr;

std::unique_ptr<Game> Game::get_instance() {
    if (instance == nullptr) {
        std::cout << "Initializing game..." << std::endl;
        // instance = new Game("MyRPGGame");
        instance = std::unique_ptr<Game>(new Game("MyRPGGame"));
    }
    return std::move(instance);
}

void Game::dispose_instance() {
    std::cout << "Nulling instance..." << '\n';
    // delete instance;
    // instance = nullptr;
}

Game::~Game() {
    delete title;
    delete current_menu;
}

Game::Game(const char* str) {
    title = str;
    sf::VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
    std::string s(title);
    window = std::make_unique<sf::RenderWindow>(videoMode, s);

    window->setVerticalSyncEnabled(false);
    window->setFramerateLimit(Constants::FPS);
    // window->setFramerateLimit(0);

    camera_view = std::make_unique<sf::View>(sf::Vector2f(0, 0), sf::Vector2f(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));

    menu_repository = std::make_unique<MenuRepository>();
    init_menus();
    menu_repository->set_menu(current_menu); // first menu is the game menu
    
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
        world_map.push_back(rowVector);
    }

    current_game_map_row = 1;
    current_game_map_col = 1;
    
    this->player = std::make_shared<Player>(PlayerType::KNIGHT,
                                            physics::Vector{ (real) Constants::SCREEN_HEIGHT/2,
                                                             (real) Constants::SCREEN_WIDTH/2 });
    
    init_world_map();
    change_current_map(current_game_map_row, current_game_map_col);

    fps_font.loadFromFile(Constants::GRAPHICS_BASE_PATH + "fonts/arial.ttf");
    fps_text.setFillColor(sf::Color::Blue);
    dt_text.setFillColor(sf::Color::Blue);
    fps_text.setFont(fps_font);
    dt_text.setFont(fps_font);
}

void Game::init_entities() {
    // initialize player's systems
     std::unique_ptr<GameEntityMovement> playerMovement = std::make_unique<GameEntityMovement>(
         player.get(), true, std::move(get_current_game_map()));
     std::unique_ptr<GameEntityBattle> playerBattle = std::make_unique<GameEntityBattle>(
         player.get()
     );
     std::unique_ptr<GameEntityMovement> enemiesMovement = std::make_unique<GameEntityMovement>(
             nullptr, false, std::move(get_current_game_map()));
    std::unique_ptr<GameEntityBattle> enemiesBattle = std::make_unique<GameEntityBattle>(
            nullptr
    );
//    auto *playerMovement = new GameEntityMovement(player.get(), true, std::move(getCurrentGameMap()));
//    auto *playerBattle = new GameEntityBattle(player.get());
//    auto *enemiesMovement = new GameEntityMovement(nullptr, false, std::move(getCurrentGameMap()));
//    auto *enemiesBattle = new GameEntityBattle(nullptr);

    player_repository = std::make_unique<PlayerRepository>(player,
                                                          std::move(playerMovement),
                                                          std::move(playerBattle),
                                                          get_current_game_map());
    
    enemies_repository = std::make_unique<EnemyRepository>(std::move(enemiesMovement),
                                                          std::move(enemiesBattle),
                                                          get_current_game_map());
}

void Game::init_menus() {
    std::vector<std::string> gameMenuItemsStrings = { "Start", "Settings", "Exit" };
    std::vector<std::string> mainMenuItemsStrings = { "Resume", "Inventory", "Settings", "Exit" };
    std::vector<std::string> characterCreationItemsStrings = { "Change Body", "Change Torso" };

    game_menu = std::make_unique<Menu>(gameMenuItemsStrings, true);
    main_menu = std::make_unique<Menu>(mainMenuItemsStrings, false);
    character_creation_menu = std::make_unique<Menu>(characterCreationItemsStrings, false);

    game_menu->add_sub_menu(main_menu.get(), 0);
    main_menu->add_sub_menu(character_creation_menu.get(), 1);
    // gameMenu->addSubMenu(settingsMenu);
    current_menu = game_menu.get(); // initial menu is gameMenu
}

void Game::start() {
    std::cout << "Press Enter to start" << std::endl;
    std::cout << "Press I or Esc to enter menu" << std::endl;
    std::cout << "Press X near an enemy to attack" << std::endl;

    init_entities();

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
        fps_text.setString("FPS: " + std::to_string(1/dt));
        dt_text.setString("DT: " + std::to_string(dt));
        dt *= multiplier;
        // setting direction to 0
        directionVector.reset_coordinates();

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

            player_repository->move(directionVector, keysPressedMap[runKey], dt);

            enemies_repository->set_game_map(get_current_game_map());

            // pressing x for attacking
            // TODO: use player repository instead of player
            if (player->is_attacking() || sf::Keyboard::isKeyPressed(attackKey)) {
                attacked = player_repository->attack(EntityMovementState::COMBAT_SLASH_ONE_HANDED, dt);
                keysPressedMap[attackKey] = true;
            }

            // pressing space for jumping
            if (player->is_jumping() || sf::Keyboard::isKeyPressed(jumpKey)) {
                player_repository->jump(directionVector, dt);
                keysPressedMap[jumpKey] = true;
            }

            // pressing escape sends to menu
            if (sf::Keyboard::isKeyPressed(pauseKey)) {
                keysPressedMap[pauseKey] = true;
                change_state(Constants::GameState::IN_MENU);
                std::cout << "In Menu (Press Space to choose)" << std::endl;
                canMove = false;
                // pressing I sends to inventory menu (to be implemented)
            } else if (eventKeyCode == inventoryKey) {
                change_state(Constants::GameState::IN_MENU);
                // currentMenu = inventoryMenu;
                std::cout << "Inventory Menu" << std::endl;
                canMove = false;
            }
            // TODO: add key for going straight to character creation
        } else if (state == Constants::GameState::IN_MENU) {
            if (sf::Keyboard::isKeyPressed(upKey)) {
                menu_repository->move_up();
            } else if (sf::Keyboard::isKeyPressed(downKey)) {
                menu_repository->move_down();
                // user chose an item menu
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                // set current menu to be the relevant one based on currentMenuItemIdx
                if (menu_repository->chose_sub_menu_item()) {
                    menu_repository->update_sub_menu(current_menu, &state);
                    //updateMenu(currentMenu, &running, &canMove, menuRepository->getMenuType());
                } else {
                    // execute the menu item functionality (cant overflow as it's defined only for non submenu indexes in the class)
                    switch (menu_repository->execute(&state)) {
                        case 0: {
                            change_state(Constants::GameState::PLAYING);
                            std::cout << "Returning to game..." << std::endl;
                            break;
                        }
                        case 1: {
                            break;
                        }
                        case 2: {
                            change_state(Constants::GameState::EXITING);
                            exit_game(&running);
                            break;
                        }
                    }
                }
                // going back one menu
            } else if (sf::Keyboard::isKeyPressed(pauseKey)) {
                // if it's the game menu then we'll exit the game
                if (menu_repository->is_game_menu()) {
                    exit_game(&running);
                    break;
                }
                // not a game menu
                current_menu = current_menu->get_parent_menu();
                menu_repository->set_menu(current_menu);
            }
        }

        // TODO: add save game functionality
        if (state == Constants::GameState::EXITING) {
            exit_game(&running);
            break;
        }

        if (state == Constants::GameState::PLAYING) {
            // make enemies move
            enemies_repository->move(player->get_position(), false, dt);
            // update physics
            for (int i = 0; i < Constants::UPDATE_ITERATIONS; i++) {
                get_current_game_map()->resolve_collisions(dt);
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

void Game::exit_game(bool *run) {
    // do all things before closing the game session (autosave or whatever)
    *run = false;
}

void Game::change_state(Constants::GameState game_state) {
    this->state = game_state;
    // TODO: change running and canMove local variables in start method
}

std::shared_ptr<GameMap> Game::get_current_game_map() {
    return world_map[current_game_map_row][current_game_map_col];
}

void Game::handle_movement() {

}

void Game::render() {
    // clear window
    window->clear();
    // TODO: handle case where state == IS_EXITING
    // draw based on game state
    if (state != Constants::GameState::PLAYING) {
        render_menu(current_menu);
    } else {
        std::shared_ptr<GameMap> map = get_current_game_map();
        // draw background
        window->draw(*(map->get_background_sprite()));
        // draw fps and dt
        window->draw(fps_text);
        window->draw(dt_text);
        // drawing undead enemies
        for (int i = 0; i < map->get_enemies().size(); i++) {
            if (!map->get_enemies()[i]->is_dead()) {
                window->draw(*(map->get_enemies()[i]->get_sprite()));
                // draw enemies' weapon
            }
        }
        window->draw(*(player->get_sprite()));
        window->draw(*(player->get_weapon()->get_sprite()));

        // drawing unreachable areas
        for (int i = 0; i < map->get_landscapes().size(); i++) {
            window->draw(*(map->get_landscapes()[i]->get_sprite()));
        }
    }
    // display all drawn sprites
    window->display();
}

void Game::render_menu(Menu *menu) {
    menu->render(player->get_position().x,
                 player->get_position().y, window.get());
}

void Game::update(const real dt) {
    // updating current map states
    get_current_game_map()->update(dt);
    // updating player state
    // playerRepository->update(dt);

    camera_view->setCenter(player->get_render_position());
    window->setView(*camera_view);
    // setting at the left hand corner in according to the player's position
    fps_text.setPosition(player->get_render_position().x - SCREEN_WIDTH/2, player->get_render_position().y - SCREEN_HEIGHT/2);
    dt_text.setPosition(player->get_render_position().x - SCREEN_WIDTH/2, player->get_render_position().y - SCREEN_HEIGHT/2 + 2*fps_text.getGlobalBounds().height);
}

void Game::update_menu(Menu *menu, bool *run, bool *move) {
    // executing command
    menu_repository->execute(&state);
    // resetting idx
    menu_repository->reset_menu_item_idx();
}

void Game::init_world_map() {
    // TODO: declare all maps here with unreachable areas and exit/enter points
    std::shared_ptr<GameMap> start_map = world_map[current_game_map_row][current_game_map_row];
    // TODO: AUTOMATE THIS SECTION
    physics::Vector bottom_left_offset = physics::Vector{-1, 1};
    physics::Vector bottom_right_offset = physics::Vector{1, 1};
    physics::Vector top_left_offset = physics::Vector{-1, -1};
    physics::Vector top_right_offset = physics::Vector{1, -1};
    real edge_length = TILE_SIZE;
    // top exit circle
    auto *start_map_top_exit_circle = new Circle(TILE_SIZE / 2, FULL_SCREEN_WIDTH / 2 + TILE_SIZE,
                                             TILE_SIZE / 2);
    start_map->set_top_exit_circle(start_map_top_exit_circle);
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
    physics::Vector house0_center = physics::Vector{535, 95};
    std::vector<physics::Vector> house0_vertices {
            house0_center + (bottom_left_offset * edge_length), // bottom left
            house0_center + (bottom_right_offset * edge_length), // bottom right
            house0_center + (top_right_offset * edge_length), // top right
            house0_center + (top_left_offset * edge_length) // top left
    };
    auto *unreachable_house0 = new LandscapeEntity(LandscapeType::TREE,
                                                 house0_center,
                                                 house0_vertices);

    start_map->add_landscape(unreachable_house0);

    std::shared_ptr<GameMap> top_map = world_map[current_game_map_row - 1][current_game_map_col];
    // bottom exit circle
    auto *top_map_bottom_exit_circle = new Circle(
    FULL_SCREEN_HEIGHT-TILE_SIZE/2, FULL_SCREEN_WIDTH/2 + TILE_SIZE, TILE_SIZE/2);
    top_map->set_bottom_exit_circle(top_map_bottom_exit_circle);
    // adding unreachable areas and landscapes
    physics::Vector unreachable_tree2_center = physics::Vector{5*FULL_SCREEN_HEIGHT/24, 6.5*FULL_SCREEN_WIDTH/8};
    std::vector<physics::Vector> unreachable_tree2_vertices = {
            unreachable_tree2_center + bottom_left_offset * edge_length, // bottom left
            unreachable_tree2_center + bottom_right_offset * edge_length, // bottom right
            unreachable_tree2_center + top_left_offset * edge_length, // top left
            unreachable_tree2_center + top_right_offset * edge_length
    };
    auto *unreachable_tree2 = new LandscapeEntity(LandscapeType::TREE,
                                                 unreachable_tree2_center,
                                                 unreachable_tree2_vertices);
    physics::Vector unreachable_tree3_center = physics::Vector{7*FULL_SCREEN_HEIGHT/12, 3*FULL_SCREEN_WIDTH/16};
    std::vector<physics::Vector> unreachable_tree3_vertices = {
            unreachable_tree3_center + bottom_left_offset * edge_length, // bottom left
            unreachable_tree3_center + bottom_right_offset * edge_length, // bottom right
            unreachable_tree3_center + top_left_offset * edge_length, // top left
            unreachable_tree3_center + top_right_offset * edge_length
    };
    auto *unreachable_tree3 = new LandscapeEntity(LandscapeType::TREE,
                                                 unreachable_tree3_center,
                                                 unreachable_tree3_vertices);
    top_map->add_landscape(unreachable_tree2);
    top_map->add_landscape(unreachable_tree3);

    // deallocate memory if needed
}

void Game::set_current_world_map_row(const int row) {
    current_game_map_row = row;
}

void Game::set_current_world_map_col(const int col) {
    current_game_map_col = col;
}

void Game::change_current_map(const int row, const int col) {
    if (row < 0 || row >= Constants::NUM_ROWS) return;
    if (col < 0 || col >= Constants::NUM_COLS) return;
    // abandoning previous map and setting its player attribute to null
    world_map[current_game_map_row][current_game_map_col]->remove_player();
    // change current map
    set_current_world_map_row(row);
    set_current_world_map_col(col);
    // initialize map (or reinitialize?)
    world_map[current_game_map_row][current_game_map_col]->init();
    // define the new game map for the entities
    if (player_repository != nullptr) player_repository->set_game_map(std::move(get_current_game_map()));
    if (enemies_repository != nullptr) enemies_repository->set_game_map(std::move(get_current_game_map()));
}
