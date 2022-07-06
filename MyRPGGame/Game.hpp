#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameMap.hpp"
#include "Player.hpp"

enum class GameState { PLAYING, PAUSED, EXITING, RESUMING, IN_MENU };

class Game {
private:
    int screenWidth, screenHeight;
    RenderWindow* window;
    VideoMode* videoMode;
    const char* title;
    GameState state;
    // entities
    Player* player;
    GameMap*** worldMap;
    int currentGameMapRow, currentGameMapCol;
public:
    Game(const char* str, int screenWidth, int screenHeight);
    ~Game() = default;
    int getScreenWidth();
    int getScreenHeight();
    RenderWindow* getWindow();
    GameMap*** getWorldMap();
    GameState getState();
    void changeState(GameState state);
    GameMap* getCurrentGameMap();
    void setPlayer(Player* player);
    void render();
    void update();
    void start();
};

#endif /* Game_hpp */
