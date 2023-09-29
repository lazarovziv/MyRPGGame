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
//    backgroundSprite = new sf::Sprite();
    backgroundSprite = std::make_unique<sf::Sprite>();

    if (texture.loadFromFile("../graphics/maps/Sample.png")) {
        std::cout << "Background loaded properly." << std::endl;
    } else std::cout << "Background NOT loaded." << std::endl;
//    texture.setSmooth(true);
    backgroundSprite->setTexture(texture);
    backgroundSprite->setOrigin(Constants::FULL_SCREEN_WIDTH/2, Constants::FULL_SCREEN_HEIGHT/2);
    backgroundSprite->setPosition(Constants::FULL_SCREEN_WIDTH/2, Constants::FULL_SCREEN_HEIGHT/2);

    // initializing gravity for map
    gravityForceGenerator = std::make_unique<physics::RigidBodyGravity>(physics::Vector{0, 0, (real) -9.81});
    forceRegistry = std::make_unique<physics::RigidBodyForceRegistry>();
    forceRegistry->addItem(&physics::Box::RIGHT_END_SCREEN, gravityForceGenerator.get());
    forceRegistry->addItem(&physics::Box::LEFT_END_SCREEN, gravityForceGenerator.get());
    forceRegistry->addItem(&physics::Box::TOP_END_SCREEN, gravityForceGenerator.get());
    forceRegistry->addItem(&physics::Box::BOTTOM_END_SCREEN, gravityForceGenerator.get());
}

// setting circles as nullptr means player can't exit from that direction
GameMap::GameMap(int row, int col, Circle *up,
                 Circle *down, Circle *right,
                 Circle *left) {
//    mapGraph = new Graph<Point *>();
    topExitCircle.reset(up);
    bottomExitCircle.reset(down);
    rightExitCircle.reset(right);
    leftExitCircle.reset(left);

}

GameMap::~GameMap() {
//    delete backgroundSprite;
    for (auto & landscape : landscapes) {
        delete landscape;
    }

    for (auto & enemy : enemiesVector) {
        delete enemy;
    }

//    delete leftExitCircle;
//    delete rightExitCircle;
//    delete topExitCircle;
//    delete bottomExitCircle;

    delete mapGraph;
}

int GameMap::getWorldMapRow() const {
    return worldMapRow;
}

int GameMap::getWorldMapCol() const {
    return worldMapCol;
}

sf::Sprite* GameMap::getBackgroundSprite() {
    return backgroundSprite.get();
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

std::vector<LandscapeEntity*> GameMap::getLandscapes() {
    return landscapes;
}

void GameMap::addLandscape(LandscapeEntity *entity) {
    landscapes.push_back(entity);
    entities.push_back(entity);
}

std::vector<NPCEnemy*> GameMap::getEnemies() {
    return enemiesVector;
}

physics::RigidBodyForceRegistry* GameMap::getForceRegistry() const {
    return forceRegistry.get();
}

void GameMap::init() {
    // don't add more enemies
    if (enemiesVector.size() >= NUM_OF_MAX_ENEMIES) return;

    // TODO: init map entities when player first enters the map
    if (!initializedMapGraph) {
//        initGraph();
        // init map
    }

    // seeding
    srand((unsigned int) time(nullptr));
    real randX = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
    real randY = generateRandom(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);

    randX = (randX/16) * 16;
    randY = (randY/16) * 16;

    auto *enemy = new NPCEnemy(NPCEnemy::WORM, physics::Vector{randX, randY});
//    enemy->increaseMaxHealthPoints(50);
    enemy->increaseDefencePoints(20);
//    enemy->increaseSpeed(13);
    addEnemy(enemy);
}

void GameMap::initGraph() {
//    initGraphVertices();
//    initGraphEdges();
    // remove all vertices which are occupied by landscapes
    for (auto &landscape : landscapes) {
        for (int row = 0; row < Constants::FULL_SCREEN_HEIGHT; row++) {
            for (int col = 0; col < Constants::FULL_SCREEN_WIDTH; col++) {
//                if (landscape->getCircle()->isPointInCircle(gameMapPoints[row][col])) {
//                    mapGraph->removeVertex(gameMapPoints[row][col]);
//                }
            }
        }
    }
    initializedMapGraph = true;
}

void GameMap::initGraphVertices() {
    // add vertices (in relevant points)
    for (int x = 0; x <= Constants::FULL_SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED; x += Constants::BASE_ENTITY_SPEED) {
        for (int y = 0; y <= Constants::FULL_SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED; y += Constants::BASE_ENTITY_SPEED) {
//            mapGraph->addVertex(gameMapPoints[x][y]);
        }
    }

}

void GameMap::initGraphEdges() {
    // add edges
    bool toAdd = true;
    for (int x = Constants::BASE_ENTITY_SPEED; x <= Constants::FULL_SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED; x += Constants::BASE_ENTITY_SPEED) {
        for (int y = Constants::BASE_ENTITY_SPEED; y <= Constants::FULL_SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED; y += Constants::BASE_ENTITY_SPEED) {
//            if (!mapGraph->isInGraph(gameMapPoints[x][y])) continue;
            toAdd = true;
            // down, right, left
            if (x == Constants::BASE_ENTITY_SPEED) {
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
                // up, right, left
            else if (x == Constants::FULL_SCREEN_HEIGHT-Constants::BASE_ENTITY_SPEED) {
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
                // up, down, left
            else if (y == Constants::FULL_SCREEN_WIDTH-Constants::BASE_ENTITY_SPEED) {
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
                toAdd = false;
            }
                // up, down, right
            else if (y == Constants::BASE_ENTITY_SPEED) {
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
                toAdd = false;
            }

            // up, down, right, left
            if (toAdd) {
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x-Constants::BASE_ENTITY_SPEED][y], 1); // up
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x+Constants::BASE_ENTITY_SPEED][y], 1); // down
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y+Constants::BASE_ENTITY_SPEED], 1); // right
//                mapGraph->addEdge(gameMapPoints[x][y], gameMapPoints[x][y-Constants::BASE_ENTITY_SPEED], 1); // left
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
    forceRegistry->addItem(enemy->getRigidBody(), gravityForceGenerator.get());
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

real GameMap::generateRandom(int min, int max) {
//    float random = ((float) rand()) / (float) RAND_MAX;
//    float diff = max - min;
//    float r = random * diff;
//    return min + r;
    return (real) min + (rand() % (max-min+1));
}


void GameMap::removePlayer() {
    // forceRegistry->removeItem(this->player->getRigidBody(), gravityForceGenerator.get());
    player.reset();
}

void GameMap::setPlayer(std::shared_ptr<Player> player) {
    this->player = player;
    forceRegistry->addItem(this->player->getRigidBody(), gravityForceGenerator.get());
}

Player *GameMap::getPlayer() {
    return player.get();
}

void GameMap::setTopExitCircle(Circle *circle) {
    topExitCircle.reset(circle);
}

void GameMap::setBottomExitCircle(Circle *circle) {
    bottomExitCircle.reset(circle);
}

void GameMap::setRightExitCircle(Circle *circle) {
    rightExitCircle.reset(circle);
}

void GameMap::setLeftExitCircle(Circle *circle) {
    leftExitCircle.reset(circle);
}

Circle *GameMap::getTopExitCircle() {
    return topExitCircle.get();
}

Circle *GameMap::getBottomExitCircle() {
    return bottomExitCircle.get();
}

Circle *GameMap::getRightExitCircle() {
    return rightExitCircle.get();
}

Circle *GameMap::getLeftExitCircle() {
    return leftExitCircle.get();
}

bool GameMap::operator==(const GameMap &map) const {
    return map.worldMapRow == worldMapRow && map.worldMapCol == worldMapCol;
}

void GameMap::update(real dt) {
    // updating physics
    forceRegistry->update(dt);

    for (auto &entity : entities) {
        // checking if enemy is dead
        if (entity->isDead()) {
            // remove it from currentEnemies and unregistering it from subject's observers
            removeEnemy(dynamic_cast<NPCEnemy*>(entity));
        } else {
            physics::resolveCollisions(player->getRigidBody(), entity->getRigidBody(), dt);
            entity->update(dt); // if enemy is still alive
        }
    }
}
