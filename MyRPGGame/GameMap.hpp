//#pragma once

#ifndef GameMap_hpp
#define GameMap_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "NPCEnemy.hpp"
#include <vector>
//class NPCEnemy;

using namespace sf;
using namespace std;

class GameMap {
private:
    // in relation to the 2d array of world map
    int worldMapRow;
    int worldMapCol;
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
    
    int numOfCurrentEnemies = 0;
    // areas where entities cannot move or reach by walking
    int numOfUnreachableAreas = 0;
    RectangleShape unreachableAreasSprites[100];
    // enemies in current
//    NPCEnemy* enemies;
    vector<NPCEnemy*> enemies;
    
    
public:
    GameMap(int row, int col);
    GameMap(int row, int col, bool up, bool down, bool right, bool left);
    ~GameMap() = default;
    int getWorldMapRow();
    int getWorldMapCol();
    
    void init();
    
    RectangleShape* getUnreachableAreasSprites();
    int getNumOfUnreachableAreas();
    void addUnreachableArea(FloatRect rect);
    bool isExitableFromLeft();
    bool isExitableFromRight();
    bool isExitableFromTop();
    bool isExitableFromBottom();
    void setIsExitableFromLeft(bool flag);
    void setIsExitableFromRight(bool flag);
    void setIsExitableFromTop(bool flag);
    void setIsExitableFromBottom(bool flag);
    
    vector<NPCEnemy*> getEnemies();
    int getNumOfCurrentEnemies();
    void addEnemy(NPCEnemy &enemy);
    
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
    
    // setters for reachable from direction
    
    bool operator == (const GameMap& map);
};

#endif /* GameMap_hpp */
