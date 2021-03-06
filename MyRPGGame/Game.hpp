//#pragma once

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameMap.hpp"
#include "Player.hpp"
#include "NPCEnemy.hpp"

enum class GameState { PLAYING, PAUSED, EXITING, RESUMING, IN_MENU };

class Game {
private:
    static Game* instance;
    RenderWindow* window;
    VideoMode* videoMode;
    const char* title;
    GameState state;
    
    // entities
    Player* player;
    
    GameMap*** worldMap;
    int currentGameMapRow, currentGameMapCol;
    
    Game(const char* str);
    
public:
    Game(Game& game) = delete;
    ~Game() = default;
    void operator=(const Game&) = delete;
    static const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
    static const int TILE_SIZE = 64;
    static Game* getInstance();
    RenderWindow* getWindow();
    GameMap*** getWorldMap();
    GameState getState();
    void changeState(GameState state);
    GameMap* getCurrentGameMap();
    void setPlayer(Player* player);
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
