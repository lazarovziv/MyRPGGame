#include "../include/GameMap.hpp"
#include <iostream>
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

GameMap::~GameMap() {
    for (int i = 0; i < unreachableAreasSprites.size(); i++) {
        delete unreachableAreasSprites.at(i);
    }

    for (int i = 0; i < enemiesVector.size(); i++) {
        delete enemiesVector.at(i);
    }

    delete leftExitCircle;
    delete rightExitCircle;
    delete topExitCircle;
    delete bottomExitCircle;
}

int GameMap::getWorldMapRow() {
    return worldMapRow;
}

int GameMap::getWorldMapCol() {
    return worldMapCol;
}

//int GameMap::getNumOfCurrentEnemies() {
//    return numOfCurrentEnemies;
//}

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

//vector<NPCEnemy*> GameMap::getEnemies() {
//    return enemies;
//}

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
    return unreachableAreasSprites.size();
}

vector<RectangleShape*> GameMap::getUnreachableAreasSprites() {
    return unreachableAreasSprites;
}

void GameMap::addUnreachableArea(IntRect* rect) {
    // creating drawable for unreachable area
    RectangleShape* rectShape = new RectangleShape(Vector2f((int) rect->width, (int) rect->height));
    rectShape->setPosition(rect->left, rect->top);
    rectShape->setFillColor(Color::Red);
    unreachableAreasSprites.push_back(rectShape);
    numOfUnreachableAreas++;
}

//NPCEnemy* GameMap::getEnemies() {
//    return enemies;
//}

vector<NPCEnemy*> GameMap::getEnemies() {
    return enemiesVector;
} 

void GameMap::init() {
    // don't add more enemies
    if (enemiesVector.size() >= NUM_OF_MAX_ENEMIES) return;
    // seeding
    srand((unsigned int) time(NULL));
    int randX = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
    int randY = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);

    IntRect* rect = new IntRect (randX, randY, Constants::TILE_SIZE, Constants::TILE_SIZE);
    Circle* circle = new Circle(randX, randY, Constants::TILE_SIZE/2);
    // assuming position is invalid
    bool validations[unreachableAreasSprites.size()];
    for (int i = 0; i < unreachableAreasSprites.size(); i++) {
        validations[i] = false;
    }
    // true if validations filled with true
    bool validPosition = false;
    while (!validPosition) {
        // checking all unreachable areas and exits (TODO: add other enemies)
        for (int i = 0; i < unreachableAreasSprites.size(); i++) {
            if (!(((IntRect) unreachableAreasSprites[i]->getGlobalBounds()).intersects(*rect))
                /*&& !circle.intersects(*topExitCircle) && !circle.intersects(*bottomExitCircle)
                && !circle.intersects(*leftExitCircle) && !circle.intersects(*rightExitCircle)*/) {
                validations[i] = true;
            } else {
                // generating a new position
                randX = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
                randY = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);
                // updating rect position
                rect->left = randX;
                rect->top = randY;
                validations[i] = false;
                // setting to -1 because i is incremented by 1
                i = -1;
            }
            // reached end of array, validating if all is well
            if (i == unreachableAreasSprites.size() - 1) {
                bool check = true;
                // check enemies positions
                for (int j = 0; j < enemiesVector.size(); j++) {
                    if (enemiesVector[j]->getRectangle().intersects(*rect)) {
                        // start over
                        for (int k = 0; k < unreachableAreasSprites.size(); k++) validations[k] = false;
                    }
                }

                for (int k = 0; k < unreachableAreasSprites.size(); k++) {
                    if (!validations[k]) check = false;
                }
                validPosition = check;
            }
        }
    }
    // deallocating memory
    delete rect;
    delete circle;

    NPCEnemy* enemy = new NPCEnemy(NPCEnemy::WORM, randX, randY);
//    enemy->increaseMaxHealthPoints(50);
    enemy->increaseDefencePoints(20);
//    enemy->increaseSpeed(13);
    enemiesVector.push_back(enemy);
}

void GameMap::addEnemy(NPCEnemy* enemy) {
    enemiesVector.push_back(enemy);
}

void GameMap::removeEnemyAtIndex(int i) {
    delete enemiesVector[i];
    enemiesVector.erase(enemiesVector.begin() + i);
}

void GameMap::removeAllEnemies() {
    for (int i = 0; i < enemiesVector.size(); i++) {
        delete enemiesVector[i];
    }
    enemiesVector.clear();
}   

int GameMap::generateRandom(int min, int max) {
//    float random = ((float) rand()) / (float) RAND_MAX;
//    float diff = max - min;
//    float r = random * diff;
//    return min + r;
    return min + (rand() % (max-min+1));
}

bool GameMap::operator==(const GameMap &map) {
    return map.worldMapRow == worldMapRow && map.worldMapCol == worldMapCol;
}
