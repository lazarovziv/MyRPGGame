//#pragma once

#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <chrono>
#include <map>
#include "MenuRepository.hpp"
#include "NPCEnemy.hpp"
#include "LandscapeEntity.hpp"
#include "PlayerRepository.hpp"
#include "EnemyRepository.hpp"
#include "Graph.hpp"

class Game {
private:
    static Game *instance;
    RenderWindow *window = nullptr;

    View *cameraView = nullptr;
    Menu *mainMenu;
    Menu *characterCreationMenu;
    Menu *gameMenu;
    Menu *currentMenu; // for changing menus when choosing submenus
    const char *title;
    Text fpsText, dtText;
    Font fpsFont;
    Constants::GameState state;
    
    // entities
    Player *player = nullptr;
    Point ***points;
    GameMap ***worldMap;

    // repositories
    MenuRepository *menuRepository;
    PlayerRepository *playerRepository;
    EnemyRepository *enemiesRepository;
//    std::vector<std::vector<GameMap*>> worldMap(3, )
    int currentGameMapRow, currentGameMapCol;
    
    explicit Game(const char *str);
    
public:
    Game(Game &game) = delete;
    ~Game();
    void operator=(const Game&) = delete;
    static const int SCREEN_WIDTH = Constants::SCREEN_WIDTH, SCREEN_HEIGHT = Constants::SCREEN_HEIGHT;
    static const int TILE_SIZE = Constants::TILE_SIZE;
    static const int FULL_SCREEN_WIDTH = Constants::FULL_SCREEN_WIDTH;
    static const int FULL_SCREEN_HEIGHT = Constants::FULL_SCREEN_HEIGHT;
    static Game* getInstance();
    static void disposeInstance();

    void changeState(Constants::GameState gameState);
    GameMap* getCurrentGameMap();
    void setCurrentWorldMapRow(int row);
    void setCurrentWorldMapCol(int col);
    
    void changeCurrentMap(int row, int col);
    
    void initWorldMap();
    void initEntities();
    void initMenus();
    void render();
    void renderMenu(Menu *menu);
    void update(Constants::MoveSuccessValues playerMoveSuccessValue, float dt);
    void updateDt();
    void updateMenu(Menu *menu, bool *run, bool *move);
    void start();
    void exitGame(bool *run);
};

#endif /* Game_hpp */
