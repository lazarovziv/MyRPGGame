//#pragma once

#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "NPCEnemy.hpp"
#include "Constants.h"
#include "LandscapeEntity.hpp"
#include "PlayerRepository.hpp"
#include "EnemyRepository.hpp"
#include "Graph.hpp"

enum class GameState { PLAYING, PAUSED, EXITING, RESUMING, IN_MENU };

class Game {
private:
    static Game *instance;
    RenderWindow *window = nullptr;
    View *cameraView = nullptr;
    const char *title;
    GameState state;
    
    // entities
    Player *player = nullptr;
    // Player player();
    Point ***points;
    GameMap ***worldMap;

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

    void changeState(GameState gameState);
    GameMap* getCurrentGameMap();
    void setCurrentWorldMapRow(int row);
    void setCurrentWorldMapCol(int col);
    
    void changeCurrentMap(int row, int col);
    
    void initWorldMap();
    void render();
    void update(Constants::MoveSuccessValues playerMoveSuccessValue);
    void start();
};

#endif /* Game_hpp */
