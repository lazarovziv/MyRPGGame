#include "GameMap.hpp"
//class NPCEnemy;

GameMap::GameMap(int row, int col) {
    worldMapRow = row;
    worldMapCol = col;
}

GameMap::GameMap(int row, int col, bool up, bool down, bool right, bool left) {
    worldMapRow = row;
    worldMapCol = col;
    exitableFromTop = up;
    exitableFromBottom = down;
    exitableFromRight = right;
    exitableFromLeft = left;
}

int GameMap::getWorldMapRow() {
    return worldMapRow;
}

int GameMap::getWorldMapCol() {
    return worldMapCol;
}

int GameMap::getNumOfCurrentEnemies() {
    return numOfCurrentEnemies;
}

bool GameMap::isExitableFromLeft() {
    return exitableFromLeft;
}

bool GameMap::isExitableFromRight() {
    return exitableFromRight;
}

bool GameMap::isExitableFromTop() {
    return exitableFromTop;
}

bool GameMap::isExitableFromBottom() {
    return exitableFromBottom;
}

vector<NPCEnemy*> GameMap::getEnemies() {
    return enemies;
}

void GameMap::setIsExitableFromLeft(bool flag) {
    exitableFromLeft = flag;
}

void GameMap::setIsExitableFromRight(bool flag) {
    exitableFromRight = flag;
}

void GameMap::setIsExitableFromTop(bool flag) {
    exitableFromTop = flag;
}

void GameMap::setIsExitableFromBottom(bool flag) {
    exitableFromBottom = flag;
}

int GameMap::getNumOfUnreachableAreas() {
    return numOfUnreachableAreas;
}

RectangleShape* GameMap::getUnreachableAreasSprites() {
    return unreachableAreasSprites;
}

void GameMap::addUnreachableArea(FloatRect rect) {
    // creating drawable for unreachable area
    RectangleShape rectShape(Vector2f(rect.width, rect.height));
    rectShape.setPosition(rect.left, rect.top);
    rectShape.setFillColor(Color::Red);
    unreachableAreasSprites[numOfUnreachableAreas] = rectShape;
    numOfUnreachableAreas++;
}

NPCEnemy* GameMap::getEnemies0() {
    return enemies0;
}

void GameMap::init() {
    // TODO: init random number of enemies and initialize all landscape and unreachable areas
    NPCEnemy* enemy = new NPCEnemy(NPCEnemy::WORM, 500, 550);
//    NPCEnemy enemy(NPCEnemy::WORM, 400, 300);
    addEnemy(enemy);
}

void GameMap::addEnemy(NPCEnemy *enemy) {
//    enemies.push_back(&enemy);
    enemies0[numOfCurrentEnemies] = *enemy;
    numOfCurrentEnemies++;
}

void GameMap::removeEnemyAtIndex(int i) {
    // delete object from memory
    enemies0[i].~NPCEnemy();
    // subtract num of enemies
    numOfCurrentEnemies--;
}

bool GameMap::operator==(const GameMap &map) {
    return map.worldMapRow == worldMapRow && map.worldMapCol == worldMapCol;
}
