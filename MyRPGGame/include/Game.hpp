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
    static Game *instance;
    std::unique_ptr<sf::RenderWindow> window = nullptr;

    std::unique_ptr<sf::View> cameraView;
    std::unique_ptr<Menu> mainMenu;
    std::unique_ptr<Menu> characterCreationMenu;
    std::unique_ptr<Menu> gameMenu;
    Menu *currentMenu; // for changing menus when choosing submenus
    const char *title;
    sf::Text fpsText, dtText;
    sf::Font fpsFont;
    Constants::GameState state;
    
    // entities
    std::shared_ptr<Player> player = nullptr;
    std::vector<std::vector<std::shared_ptr<GameMap>>> worldMap;

    // repositories
    std::unique_ptr<MenuRepository> menuRepository;
    std::unique_ptr<PlayerRepository> playerRepository;
    std::unique_ptr<EnemyRepository> enemiesRepository;
//    std::vector<std::vector<GameMap*>> worldMap(3, )
    int currentGameMapRow, currentGameMapCol;
    
    explicit Game(const char *str);
    
public:
    Game(Game &game) = delete;
    ~Game();
    void operator=(const Game&) = delete;
    static constexpr real SCREEN_WIDTH = Constants::SCREEN_WIDTH, SCREEN_HEIGHT = Constants::SCREEN_HEIGHT;
    static constexpr real TILE_SIZE = Constants::TILE_SIZE;
    static constexpr real FULL_SCREEN_WIDTH = Constants::FULL_SCREEN_WIDTH;
    static constexpr real FULL_SCREEN_HEIGHT = Constants::FULL_SCREEN_HEIGHT;
    static Game* getInstance();
    static void disposeInstance();

    void changeState(Constants::GameState gameState);
    std::shared_ptr<GameMap> getCurrentGameMap();
    void setCurrentWorldMapRow(int row);
    void setCurrentWorldMapCol(int col);
    
    void changeCurrentMap(int row, int col);
    
    void initWorldMap();
    void initEntities();
    void initMenus();
    void render();
    void renderMenu(Menu *menu);
    void update(real dt);
    void updateMenu(Menu *menu, bool *run, bool *move);
    void start();
    void exitGame(bool *run);
};

#endif /* Game_hpp */
