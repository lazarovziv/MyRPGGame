#ifndef GameMap_hpp
#define GameMap_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

class GameMap {
private:
    // in relation to the 2d array of world map
    int worldMapRow;
    int worldMapCol;
    bool reachableFromLeft, reachableFromRight, reachableFromTop, reachableFromBottom;
    bool exitableFromLeft, exitableFromRight, exitableFromTop, exitableFromBottom;
    Vector2f leftEnterPoint, leftExitPoint;
    Vector2f rightEnterPoint, rightExitPoint;
    Vector2f topEnterPoint, topExitPoint;
    Vector2f bottomEnterPoint, bottomExitPoint;
    // areas where entities cannot move or reach by walking
    int numOfUnreachableAreas = 0;
    FloatRect unreachableAreas[100];
    RectangleShape unreachableAreasSprites[100];
    
    
public:
    GameMap(int row, int col);
    ~GameMap() = default;
    int getWorldMapRow();
    int getWorldMapCol();
    Vector2f getLeftEnterPoint();
    Vector2f getLeftExitPoint();
    Vector2f getRightEnterPoint();
    Vector2f getRightExitPoint();
    Vector2f getTopEnterPoint();
    Vector2f getTopExitPoint();
    Vector2f getBottomEnterPoint();
    Vector2f getBottomExitPoint();
    FloatRect* getUnreachableAreas();
    RectangleShape* getUnreachableAreasSprites();
    int getNumOfUnreachableAreas();
    void addUnreachableArea(FloatRect rect);
    bool isReachableFromLeft();
    bool isReachableFromRight();
    bool isReachableFromTop();
    bool isReachableFromBottom();
    bool isExitableFromLeft();
    bool isExitableFromRight();
    bool isExitableFromTop();
    bool isExitableFromBottom();
    // setters for reachable from direction
};

#endif /* GameMap_hpp */
