//#pragma once

#ifndef GameMap_hpp
#define GameMap_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "NPCEnemy.hpp"
#include "Player.hpp"
#include <vector>
#include "Circle.hpp"
#include "Constants.h"
#include "LandscapeEntity.hpp"
#include "Graph.hpp"
class NPCEnemy;

class GameMap {
private:
    // in relation to the 2d array of world map
    int worldMapRow;
    int worldMapCol;
    Point ***gameMapPoints;
    Graph<Point *> *mapGraph;
    bool initializedMapGraph = false; // used also for indicator for player's first arrival

    bool exitableFromLeft, exitableFromRight, exitableFromTop, exitableFromBottom;
    
    Circle *leftExitCircle = nullptr;
    Circle *rightExitCircle = nullptr;
    Circle *topExitCircle = nullptr;
    Circle *bottomExitCircle = nullptr;

    sf::Texture texture;
    sf::Sprite *backgroundSprite;

    const int NUM_OF_MAX_ENEMIES = 3;
    // areas where entities cannot move or reach by walking
    std::vector<LandscapeEntity *> landscapes;
    // enemies in current map
    std::vector<NPCEnemy *> enemiesVector;
    std::vector<GameEntity *> entities;
    Player *player;
    
public:
    GameMap(int row, int col);
    GameMap(int row, int col, bool up, bool down, bool right, bool left);
    GameMap(int row, int col, bool up, bool down, bool right, bool left, Point ***points);
    GameMap(int row, int col, Circle *up, Circle *down, Circle *right, Circle *left, Point ***points);
    ~GameMap();

    int getWorldMapRow() const;
    int getWorldMapCol() const;
    
    void init();
    void initGraph();
    void initGraphVertices();
    void initGraphEdges();

    Graph<Point *> *getMapGraph();

    sf::Sprite* getBackgroundSprite();

    std::vector<LandscapeEntity *> getLandscapes();
    void addLandscape(LandscapeEntity* entity);
    bool isExitableFromLeft() const;
    bool isExitableFromRight() const;
    bool isExitableFromTop() const;
    bool isExitableFromBottom() const;
    // for changing maps during gameplay
    void setIsExitableFromLeft(bool flag);
    void setIsExitableFromRight(bool flag);
    void setIsExitableFromTop(bool flag);
    void setIsExitableFromBottom(bool flag);

    Player *getPlayer();
    void setPlayer(Player *player);
    void removePlayer();
    std::vector<NPCEnemy *> getEnemies();
    void addEnemy(NPCEnemy *enemy);
    void removeEnemyAtIndex(int i);
    void removeEnemy(NPCEnemy *enemy);
    void removeAllEnemies();

    void setTopExitCircle(Circle *circle);
    void setBottomExitCircle(Circle *circle);
    void setRightExitCircle(Circle *circle);
    void setLeftExitCircle(Circle *circle);

    Circle *getTopExitCircle();
    Circle *getBottomExitCircle();
    Circle *getRightExitCircle();
    Circle *getLeftExitCircle();

    static int generateRandom(int min, int max);

    void update(real dt);
    
    bool operator ==(const GameMap& map) const;
};

#endif /* GameMap_hpp */
