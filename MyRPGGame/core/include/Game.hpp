//#pragma once

#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <chrono>
#include <map>
#include <memory>
#include "MenuRepository.hpp"
#include "NPCEnemy.hpp"
#include "LandscapeEntity.hpp"
#include "PlayerRepository.hpp"
#include "EnemyRepository.hpp"
#include "Graph.hpp"

class Game {
private:
    // static Game *instance;
    static std::unique_ptr<Game> instance;
    std::unique_ptr<sf::RenderWindow> window = nullptr;

    std::unique_ptr<sf::View> camera_view;
    std::unique_ptr<Menu> main_menu;
    std::unique_ptr<Menu> character_creation_menu;
    std::unique_ptr<Menu> game_menu;
    Menu *current_menu; // for changing menus when choosing submenus
    const char *title;
    sf::Text fps_text, dt_text;
    sf::Font fps_font;
    Constants::GameState state;
    
    // entities
    std::shared_ptr<Player> player = nullptr;
    std::vector<std::vector<std::shared_ptr<GameMap>>> world_map;

    // repositories
    std::unique_ptr<MenuRepository> menu_repository;
    std::unique_ptr<PlayerRepository> player_repository;
    std::unique_ptr<EnemyRepository> enemies_repository;
//    std::vector<std::vector<GameMap*>> worldMap(3, )
    int current_game_map_row, current_game_map_col;
    
    explicit Game(const char *str);

    static void exit_game(bool *run);

    void change_state(Constants::GameState game_state);
    std::shared_ptr<GameMap> get_current_game_map();
    void set_current_world_map_row(int row);
    void set_current_world_map_col(int col);

    void change_current_map(int row, int col);

    void init_world_map();
    void init_entities();
    void init_menus();

    void handle_movement();

    void render();
    void render_menu(Menu *menu);
    void update(real dt);
    void update_menu(Menu *menu, bool *run, bool *move);
    
public:
    Game(Game &game) = delete;
    ~Game();
    void operator=(const Game&) = delete;
    static constexpr real SCREEN_WIDTH = Constants::SCREEN_WIDTH, SCREEN_HEIGHT = Constants::SCREEN_HEIGHT;
    static constexpr real TILE_SIZE = Constants::TILE_SIZE;
    static constexpr real FULL_SCREEN_WIDTH = Constants::FULL_SCREEN_WIDTH;
    static constexpr real FULL_SCREEN_HEIGHT = Constants::FULL_SCREEN_HEIGHT;
    static std::unique_ptr<Game> get_instance();
    static void dispose_instance();

    void start();
};

#endif /* Game_hpp */
