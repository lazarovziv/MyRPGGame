#include "../include/GameMap.hpp"
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
    backgroundSprite = new Sprite();
    if (texture.loadFromFile("../graphics/trees/grass_bg_800x640.png")) {
        std::cout << "Background loaded properly." << endl;
    } else std::cout << "Background NOT loaded." << endl;
//    texture.setSmooth(true);
    backgroundSprite->setTexture(texture);
    backgroundSprite->setOrigin(Constants::SCREEN_WIDTH/2, Constants::SCREEN_HEIGHT/2);
    backgroundSprite->setPosition(Constants::SCREEN_WIDTH/2, Constants::SCREEN_HEIGHT/2);
}

GameMap::GameMap(int row, int col, bool up, bool down, bool right, bool left, Point ***points) : GameMap(row, col, up, down, right, left) {
    gameMapPoints = points;
}

GameMap::~GameMap() {
    delete backgroundSprite;
    for (int i = 0; i < landscapes.size(); i++) {
        delete landscapes[i];
    }

    for (int i = 0; i < enemiesVector.size(); i++) {
        delete enemiesVector[i];
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

Sprite* GameMap::getBackgroundSprite() {
    return backgroundSprite;
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

vector<LandscapeEntity*> GameMap::getLandscapes() {
    return landscapes;
}

void GameMap::addLandscape(LandscapeEntity *entity) {
    landscapes.push_back(entity);
}

vector<NPCEnemy*> GameMap::getEnemies() {
    return enemiesVector;
} 

void GameMap::init() {
    // don't add more enemies
    if (enemiesVector.size() >= NUM_OF_MAX_ENEMIES) return;
    // seeding
    srand((unsigned int) time(nullptr));
    int randX = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
    int randY = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);

    randX = (randX/16) * 16;
    randY = (randY/16) * 16;

    auto *rect = new IntRect(randX, randY, Constants::TILE_SIZE, Constants::TILE_SIZE);
    auto *circle = new Circle(randX, randY, Constants::TILE_SIZE/2);
    // assuming position is invalid
    bool validations[landscapes.size()];
    for (int i = 0; i < landscapes.size(); i++) {
        validations[i] = false;
    }
    // true if validations filled with true
    bool validPosition = false;
    while (!validPosition) {
        // checking all unreachable areas and exits (TODO: add other enemies)
        for (int i = 0; i < landscapes.size(); i++) {
            if (!(((IntRect) landscapes[i]->getRectangle()).intersects(*rect))
                /*&& !circle.intersects(*topExitCircle) && !circle.intersects(*bottomExitCircle)
                && !circle.intersects(*leftExitCircle) && !circle.intersects(*rightExitCircle)*/) {
                validations[i] = true;
            } else {
                // generating a new position
                randX = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
                randY = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);
                randX = (randX/16) * 16;
                randY = (randY/16) * 16;
                // updating rect position
                rect->left = randX;
                rect->top = randY;
                validations[i] = false;
                // setting to -1 because i is incremented by 1
                i = -1;
            }
            // reached end of array, validating if all is well
            if (i == landscapes.size() - 1) {
                bool check = true;
                // check enemies positions
                for (int j = 0; j < enemiesVector.size(); j++) {
                    if (enemiesVector[j]->getRectangle().intersects(*rect)) {
                        // start over
                        for (int k = 0; k < landscapes.size(); k++) validations[k] = false;
                    }
                }

                for (int k = 0; k < landscapes.size(); k++) {
                    if (!validations[k]) check = false;
                }
                validPosition = check;
            }
        }
    }

    // deallocating memory
    delete rect;
    delete circle;

//    auto* enemy = new NPCEnemy(NPCEnemy::WORM, randX, randY);
    auto *enemy = new NPCEnemy(NPCEnemy::WORM, gameMapPoints[randY][randX]);
//    enemy->increaseMaxHealthPoints(50);
    enemy->increaseDefencePoints(20);
//    enemy->increaseSpeed(13);
    addEnemy(enemy);
}

void GameMap::addEnemy(NPCEnemy *enemy) {
    enemiesVector.push_back(enemy);
}

void GameMap::removeEnemyAtIndex(int i) {
//    delete enemiesVector[i];
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
    return min + (random() % (max-min+1));
}

void GameMap::update() {
    for (int i = 0; i < enemiesVector.size(); i++) {
        // checking if enemy is dead
        if (enemiesVector[i]->isDead()) {
            // remove it from currentEnemies
            removeEnemyAtIndex(i);
            continue;
            // if enemy is still alive
        } else enemiesVector[i]->update();
    }
}

bool GameMap::operator==(const GameMap &map) const {
    return map.worldMapRow == worldMapRow && map.worldMapCol == worldMapCol;
}
