//#pragma once

#ifndef GameMap_hpp
#define GameMap_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "NPCEnemy.hpp"
#include <vector>
#include "Circle.hpp"
#include "Constants.h"
#include "LandscapeEntity.hpp"
#include "Graph.hpp"
class NPCEnemy;
class Player;

using namespace sf;
using namespace std;

class GameMap {
private:
    // in relation to the 2d array of world map
    int worldMapRow;
    int worldMapCol;
    Point ***gameMapPoints;
    bool exitableFromLeft, exitableFromRight, exitableFromTop, exitableFromBottom;
    // enter coordinates
    // no need for x
    float leftEnterMinY, leftEnterMaxY;
    float leftExitMinY, leftExitMaxY;
    float rightEnterMinY, rightEnterMaxY;
    float rightExitMinY, rightExitMaxY;
    // no need for y
    float topEnterMinX, topEnterMaxX;
    float bottomEnterMinX, bottomEnterMaxX;
    float topExitMinX, topExitMaxX;
    float bottomExitMinX, bottomExitMaxX;
    
    Circle *leftExitCircle = nullptr;
    Circle *rightExitCircle = nullptr;
    Circle *topExitCircle = nullptr;
    Circle *bottomExitCircle = nullptr;

    Texture texture;
    Sprite *backgroundSprite;
    
    int numOfCurrentEnemies = 0;
    const int NUM_OF_MAX_ENEMIES = 3;
    // areas where entities cannot move or reach by walking
    vector<LandscapeEntity *> landscapes;
    // enemies in current map
    vector<NPCEnemy *> enemiesVector;
    vector<GameEntity *> entities;
    Player *player;
    
public:
    GameMap(int row, int col);
    GameMap(int row, int col, bool up, bool down, bool right, bool left);
    GameMap(int row, int col, bool up, bool down, bool right, bool left, Point ***points);
    ~GameMap();
    int getWorldMapRow();
    int getWorldMapCol();
    
    void init();

    Sprite* getBackgroundSprite();

    vector<LandscapeEntity *> getLandscapes();
    void addLandscape(LandscapeEntity* entity);
    bool isExitableFromLeft();
    bool isExitableFromRight();
    bool isExitableFromTop();
    bool isExitableFromBottom();
    void setIsExitableFromLeft(bool flag);
    void setIsExitableFromRight(bool flag);
    void setIsExitableFromTop(bool flag);
    void setIsExitableFromBottom(bool flag);

    Player *getPlayer();
    void setPlayer(Player *player);
    void removePlayer();
    vector<NPCEnemy *> getEnemies();
    void addEnemy(NPCEnemy *enemy);
    void removeEnemyAtIndex(int i);
    void removeEnemy(NPCEnemy *enemy);
    void removeAllEnemies();
    
    static int generateRandom(int min, int max);

    void update();
    
    void setTopExit(float minX, float maxX) {
        topExitMinX = minX;
        topExitMaxX = maxX;
        // top side of screen
        topExitCircle = new Circle((topExitMinX + topExitMaxX)/2, 0, (topExitMinX - topExitMaxX)/2);
    }
    
    void setBottomExit(float minX, float maxX) {
        bottomExitMinX = minX;
        bottomExitMaxX = maxX;
        // bottom side of screen
        bottomExitCircle = new Circle((topExitMinX + topExitMaxX)/2, (float) Constants::SCREEN_HEIGHT, (topExitMinX - topExitMaxX)/2);
    }
    
    void setLeftExit(float minY, float maxY) {
        leftExitMinY = minY;
        leftExitMaxY = maxY;
        // left side of screen
        leftExitCircle = new Circle(0, (leftExitMinY + leftExitMaxY)/2, (leftExitMinY - leftExitMaxY)/2);
    }
    
    void setRightExit(float minY, float maxY) {
        rightExitMinY = minY;
        rightExitMaxY = maxY;
        // right side of screen
        rightExitCircle = new Circle(Constants::SCREEN_WIDTH, (rightExitMinY + rightExitMaxY)/2, (rightExitMinY - rightExitMaxY)/2);
    }
    
    float getLeftEnterMinY() {
        return leftEnterMinY;
    }
    
    float getLeftEnterMaxY() {
        return leftEnterMaxY;
    }
    
    float getLeftExitMinY() {
        return leftExitMinY;
    }
    
    float getLeftExitMaxY() {
        return leftExitMaxY;
    }
    
    float getRightEnterMinY() {
        return rightEnterMinY;
    }
    
    float getRightEnterMaxY() {
        return rightEnterMaxY;
    }
    
    float getRightExitMinY() {
        return rightExitMinY;
    }
    
    float getRightExitMaxY() {
        return rightExitMaxY;
    }
    
    float getTopEnterMinX() {
        return topEnterMinX;
    }
    
    float getTopEnterMaxX() {
        return topEnterMaxX;
    }
    
    float getTopExitMinX() {
        return topExitMinX;
    }
    
    float getTopExitMaxX() {
        return topExitMaxX;
    }
    
    float getBottomEnterMinX() {
        return bottomEnterMinX;
    }
    
    float getBottomEnterMaxX() {
        return bottomEnterMaxX;
    }
    
    float getBottomExitMinX() {
        return bottomExitMinX;
    }
    
    float getBottomExitMaxX() {
        return bottomExitMaxX;
    }
    
    void setLeftEnterMinY(float y) {
        leftEnterMinY = y;
    }
    
    void setLeftEnterMaxY(float y) {
        leftEnterMaxY = y;
    }
    
    void setLeftExitMinY(float y) {
        leftExitMinY = y;
    }
    
    void setLeftExitMaxY(float y) {
        leftExitMaxY = y;
    }
    
    void setRightEnterMinY(float y) {
        rightEnterMinY = y;
    }
    
    void setRightEnterMaxY(float y) {
        rightEnterMaxY = y;
    }
    
    void setRightExitMinY(float y) {
        rightExitMinY = y;
    }
    
    void setRightExitMaxY(float y) {
        rightExitMaxY = y;
    }
    
    void setTopEnterMinX(float x) {
        topEnterMinX = x;
    }
    
    void setTopEnterMaxX(float x) {
        topEnterMaxX = x;
    }
    
    void setTopExitMinX(float x) {
        topExitMinX = x;
    }
    
    void setTopExitMaxX(float x) {
        topExitMaxX = x;
    }
    
    void setBottomEnterMinX(float x) {
        bottomEnterMinX = x;
    }
    
    void setBottomEnterMaxX(float x) {
        bottomEnterMaxX = x;
    }
    
    void setBottomExitMinX(float x) {
        bottomExitMinX = x;
    }
    
    void setBottomExitMaxX(float x) {
        bottomExitMaxX = x;
    }
    
    bool operator == (const GameMap& map) const;
};

#endif /* GameMap_hpp */
