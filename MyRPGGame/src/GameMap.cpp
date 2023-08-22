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
    if (texture.loadFromFile("../graphics/maps/map_1_1.png")) {
        std::cout << "Background loaded properly." << endl;
    } else std::cout << "Background NOT loaded." << endl;
//    texture.setSmooth(true);
    backgroundSprite->setTexture(texture);
    backgroundSprite->setOrigin(Constants::FULL_SCREEN_WIDTH/2, Constants::FULL_SCREEN_HEIGHT/2);
    backgroundSprite->setPosition(Constants::FULL_SCREEN_WIDTH/2, Constants::FULL_SCREEN_HEIGHT/2);
}

GameMap::GameMap(int row, int col, bool up, bool down, bool right, bool left, Point ***points) : GameMap(row, col, up, down, right, left) {
    gameMapPoints = points;
    mapGraph = new Graph<Point *>();
}

// setting circles as nullptr means player can't exit from that direction
GameMap::GameMap(int row, int col, Circle *up,
                 Circle *down, Circle *right,
                 Circle *left, Point ***points) : GameMap(row, col,
                                                          up != nullptr, down != nullptr,
                                                          right != nullptr, left != nullptr) {
    gameMapPoints = points;
    topExitCircle = up;
    bottomExitCircle = down;
    rightExitCircle = right;
    leftExitCircle = left;
}

GameMap::~GameMap() {
    delete backgroundSprite;
    for (auto & landscape : landscapes) {
        delete landscape;
    }

    for (auto & enemy : enemiesVector) {
        delete enemy;
    }

    delete leftExitCircle;
    delete rightExitCircle;
    delete topExitCircle;
    delete bottomExitCircle;

    delete mapGraph;
}

int GameMap::getWorldMapRow() const {
    return worldMapRow;
}

int GameMap::getWorldMapCol() const {
    return worldMapCol;
}

Sprite* GameMap::getBackgroundSprite() {
    return backgroundSprite;
}

bool GameMap::isExitableFromLeft() const {
    return exitableFromLeft;
}

bool GameMap::isExitableFromRight() const {
    return exitableFromRight;
}

bool GameMap::isExitableFromTop() const {
    return exitableFromTop;
}

bool GameMap::isExitableFromBottom() const {
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
    entities.push_back(entity);
}

vector<NPCEnemy*> GameMap::getEnemies() {
    return enemiesVector;
} 

void GameMap::init() {
    // don't add more enemies
    if (enemiesVector.size() >= NUM_OF_MAX_ENEMIES) return;

    // TODO: init map entities when player first enters the map
    if (!initializedMapGraph) {
        initGraph();
        // init map
    }

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

void GameMap::initGraph() {
    initGraphVertices();
    initGraphEdges();
    // remove all vertices which are occupied by landscapes
    for (auto &landscape : landscapes) {
        for (int row = 0; row < Constants::FULL_SCREEN_HEIGHT; row++) {
            for (int col = 0; col < Constants::FULL_SCREEN_WIDTH; col++) {
                if (landscape->getCircle()->isPointInCircle(gameMapPoints[row][col])) {
                    mapGraph->removeVertex(gameMapPoints[row][col]);
                }
            }
        }
    }
    initializedMapGraph = true;
}

void GameMap::initGraphVertices() {
    // add vertices (in relevant points)
    for (int x = 0; x <= Constants::FULL_SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED; x += Constants::BASE_ENTITY_SPEED) {
        for (int y = 0; y <= Constants::FULL_SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED; y += Constants::BASE_ENTITY_SPEED) {
            mapGraph->addVertex(gameMapPoints[x][y]);
        }
    }

}

void GameMap::initGraphEdges() {
    // add edges
    bool toAdd = true;
    for (int x = Constants::BASE_ENTITY_SPEED; x <= Constants::FULL_SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED; x += Constants::BASE_ENTITY_SPEED) {
        for (int y = Constants::BASE_ENTITY_SPEED; y <= Constants::FULL_SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED; y += Constants::BASE_ENTITY_SPEED) {
            if (!mapGraph->isInGraph(gameMapPoints[x][y])) continue;
            toAdd = true;
            // down, right, left
            if (x == Constants::BASE_ENTITY_SPEED) {
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
                // up, right, left
            else if (x == Constants::FULL_SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED) {
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
                // up, down, left
            else if (y == Constants::FULL_SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED) {
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
                // up, down, right
            else if (y == Constants::BASE_ENTITY_SPEED) {
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                toAdd = false;
            }

            // up, down, right, left
            if (toAdd) {
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
            }
        }
    }
}

Graph<Point *> *GameMap::getMapGraph() {
    return mapGraph;
}

void GameMap::addEnemy(NPCEnemy *enemy) {
    enemiesVector.push_back(enemy);
    entities.push_back(enemy);
    // registering observer
    if (player != nullptr) player->registerObserver(enemy);
}

void GameMap::removeEnemyAtIndex(int i) {
//    delete enemiesVector[i];
    enemiesVector.erase(enemiesVector.begin() + i);
}

void GameMap::removeEnemy(NPCEnemy *enemy) {
    enemiesVector.erase(std::find(enemiesVector.begin(), enemiesVector.end(), enemy));
    entities.erase(std::find(entities.begin(), entities.end(), enemy));
    // unregistering enemy from player's observers
    player->unregisterObserver(enemy);
}

void GameMap::removeAllEnemies() {
    for (auto &enemy : enemiesVector) {
        delete enemy;
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
    for (auto &enemy : enemiesVector) {
        // checking if enemy is dead
        if (enemy->isDead()) {
            // remove it from currentEnemies and unregistering it from subject's observers
            removeEnemy(enemy);
        } else enemy->update(gameMapPoints); // if enemy is still alive
    }
}

void GameMap::removePlayer() {
    player = nullptr;
}

void GameMap::setPlayer(Player *player) {
    this->player = player;
}

Player *GameMap::getPlayer() {
    return player;
}

void GameMap::setTopExitCircle(Circle *circle) {
    topExitCircle = circle;
}

void GameMap::setBottomExitCircle(Circle *circle) {
    bottomExitCircle = circle;
}

void GameMap::setRightExitCircle(Circle *circle) {
    rightExitCircle = circle;
}

void GameMap::setLeftExitCircle(Circle *circle) {
    leftExitCircle = circle;
}

Circle *GameMap::getTopExitCircle() {
    return topExitCircle;
}

Circle *GameMap::getBottomExitCircle() {
    return bottomExitCircle;
}

Circle *GameMap::getRightExitCircle() {
    return rightExitCircle;
}

Circle *GameMap::getLeftExitCircle() {
    return leftExitCircle;
}

bool GameMap::operator==(const GameMap &map) const {
    return map.worldMapRow == worldMapRow && map.worldMapCol == worldMapCol;
}
