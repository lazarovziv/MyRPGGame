#include "GameMap.hpp"

GameMap::GameMap(int row, int col) {
    worldMapRow = row;
    worldMapCol = col;
}

int GameMap::getWorldMapRow() {
    return worldMapRow;
}

int GameMap::getWorldMapCol() {
    return worldMapCol;
}

Vector2f GameMap::getLeftEnterPoint() {
    return leftEnterPoint;
}

Vector2f GameMap::getLeftExitPoint() {
    return leftExitPoint;
}

Vector2f GameMap::getRightEnterPoint() {
    return rightEnterPoint;
}

Vector2f GameMap::getRightExitPoint() {
    return rightExitPoint;
}

Vector2f GameMap::getTopEnterPoint() {
    return topEnterPoint;
}

Vector2f GameMap::getTopExitPoint() {
    return topExitPoint;
}

Vector2f GameMap::getBottomEnterPoint() {
    return bottomEnterPoint;
}

Vector2f GameMap::getBottomExitPoint() {
    return bottomExitPoint;
}

FloatRect* GameMap::getUnreachableAreas() {
    return unreachableAreas;
}

bool GameMap::isReachableFromLeft() {
    return reachableFromLeft;
}

bool GameMap::isReachableFromRight() {
    return reachableFromRight;
}

bool GameMap::isReachableFromTop() {
    return reachableFromTop;
}

bool GameMap::isReachableFromBottom() {
    return reachableFromBottom;
}

int GameMap::getNumOfUnreachableAreas() {
    return numOfUnreachableAreas;
}

void GameMap::addUnreachableArea(FloatRect rect) {
    unreachableAreas[numOfUnreachableAreas] = rect;
    numOfUnreachableAreas++;
}
