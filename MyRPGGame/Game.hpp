#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "GameMap.hpp"

enum class GameState { PLAYING, PAUSED, EXITING, RESUMING, IN_MENU };

class Game {
private:
    int screenWidth, screenHeight;
    RenderWindow* window;
    VideoMode* videoMode;
    const char* title;
    GameState state;
    // entities
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
    void render();
    void update();
    void changeState(GameState state);
    GameMap* getCurrentGameMap();
};

#endif /* Game_hpp */
