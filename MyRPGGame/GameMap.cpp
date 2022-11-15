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

NPCEnemy* GameMap::getEnemies() {
    return enemies;
}

void GameMap::init() {
    // seeding
    srand((unsigned int) time(NULL));
    float randX = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
    float randY = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);

    FloatRect rect(randX, randY, Constants::TILE_SIZE, Constants::TILE_SIZE);
    Circle circle(randX, randY, Constants::TILE_SIZE/2);
    // assuming position is invalid
    bool validations[numOfUnreachableAreas];
    for (int i = 0; i < numOfUnreachableAreas; i++) {
        validations[i] = false;
    }
    // true if validations filled with true
    bool validPosition = false;
    while (!validPosition) {
        // checking all unreachable areas and exits
        for (int i = 0; i < numOfUnreachableAreas; i++) {
            if (!unreachableAreasSprites[i].getGlobalBounds().intersects(rect)
                /*&& !circle.intersects(*topExitCircle) && !circle.intersects(*bottomExitCircle)
                && !circle.intersects(*leftExitCircle) && !circle.intersects(*rightExitCircle)*/) {
                validations[i] = true;
            } else {
                // generating a new position
                randX = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
                randY = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);
                // updating rect position
                rect.left = randX;
                rect.top = randY;
                validations[i] = false;
                // checking previous areas
                for (int j = 0; j <= i; j++) {
                    if (!unreachableAreasSprites[j].getGlobalBounds().intersects(rect)
                        /*&& !circle.intersects(*topExitCircle) && !circle.intersects(*bottomExitCircle)
                        && !circle.intersects(*leftExitCircle) && !circle.intersects(*rightExitCircle)*/) {
                        validations[j] = true;
                    } else {
                        validations[j] = false;
                        i = i - 1;
                        break;
                    }
                }
            }
            // reached end of array, validating if all is well
            if (i == numOfUnreachableAreas - 1) {
                bool check = true;
                for (int k = 0; k < numOfUnreachableAreas; k++) {
                    if (!validations[k]) check = false;
                }
                validPosition = check;
            }
        }
    }
    
    NPCEnemy* enemy = new NPCEnemy(NPCEnemy::WORM, randX, randY);
    enemy->increaseMaxHealthPoints(50);
    enemy->increaseDefencePoints(50);
    addEnemy(enemy);
}

void GameMap::addEnemy(NPCEnemy *enemy) {
//    enemies.push_back(&enemy);
    enemies[numOfCurrentEnemies] = *enemy;
    numOfCurrentEnemies++;
}

void GameMap::removeEnemyAtIndex(int i) {
    // delete object from memory
    enemies[i].~NPCEnemy();
    // subtract num of enemies
    numOfCurrentEnemies--;
}

float GameMap::generateRandom(float min, float max) {
//    float random = ((float) rand()) / (float) RAND_MAX;
//    float diff = max - min;
//    float r = random * diff;
//    return min + r;
    return min + ((((float) rand()) / (float) RAND_MAX) * (max - min));
}

bool GameMap::operator==(const GameMap &map) {
    return map.worldMapRow == worldMapRow && map.worldMapCol == worldMapCol;
}
