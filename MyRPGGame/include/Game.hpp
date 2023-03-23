//#pragma once

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameMap.hpp"
#include "Player.hpp"
#include "NPCEnemy.hpp"
#include "Constants.h"
#include "LandscapeEntity.hpp"
#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

enum class GameState { PLAYING, PAUSED, EXITING, RESUMING, IN_MENU };

class Game {
private:
    static Game *instance;
    RenderWindow *window = nullptr;
    const char *title;
    GameState state;
    
    // entities
     Player *player = nullptr;
    // Player player();
    
    GameMap ***worldMap;
//    std::vector<std::vector<GameMap*>> worldMap(3, )
    int currentGameMapRow, currentGameMapCol;
    
    Game(const char *str);
    
public:
    Game(Game &game) = delete;
    ~Game();
    void operator=(const Game&) = delete;
    static const int SCREEN_WIDTH = Constants::SCREEN_WIDTH, SCREEN_HEIGHT = Constants::SCREEN_HEIGHT;
    static const int TILE_SIZE = Constants::TILE_SIZE;
    static Game* getInstance();
    static void disposeInstance();
    RenderWindow* getWindow();
    GameMap*** getWorldMap();
    GameState getState();
    Player* getPlayer();
    void changeState(GameState state);
    GameMap* getCurrentGameMap();
    int getCurrentWorldMapRow();
    int getCurrentWorldMapCol();
    void setCurrentWorldMapRow(int row);
    void setCurrentWorldMapCol(int col);
    
    void changeCurrentMap(int row, int col);
    
    void initWorldMap();
    void render();
    void update();
    void start();
};

#endif /* Game_hpp */
