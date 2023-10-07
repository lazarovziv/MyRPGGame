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
//    gravityForceGenerator = std::make_unique<physics::RigidBodyGravity>(physics::Vector{0,  (real) -9.81});
    gravityForceGenerator = std::make_unique<physics::RigidBodyGravity>(physics::Vector::ZERO);
    forceRegistry = std::make_unique<physics::RigidBodyForceRegistry>();
    forceRegistry->addItem(&physics::Polygon::RIGHT_END_SCREEN, gravityForceGenerator.get());
    forceRegistry->addItem(&physics::Polygon::LEFT_END_SCREEN, gravityForceGenerator.get());
    forceRegistry->addItem(&physics::Polygon::TOP_END_SCREEN, gravityForceGenerator.get());
    forceRegistry->addItem(&physics::Polygon::BOTTOM_END_SCREEN, gravityForceGenerator.get());
    bodies.push_back(&physics::Polygon::RIGHT_END_SCREEN);
    bodies.push_back(&physics::Polygon::LEFT_END_SCREEN);
    bodies.push_back(&physics::Polygon::TOP_END_SCREEN);
    bodies.push_back(&physics::Polygon::BOTTOM_END_SCREEN);
    auto *testCircle = new physics::Circle(255, 282, 0, 64);
    testCircle->setMass(1000);
    bodies.push_back(testCircle);
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
    bodies.push_back(entity->getRigidBody());
    forceRegistry->addItem(entity->getRigidBody(), gravityForceGenerator.get());
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

void GameMap::addEnemy(NPCEnemy *enemy) {
    enemiesVector.push_back(enemy);
    entities.push_back(enemy);
    bodies.push_back(enemy->getRigidBody());
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
    bodies.erase(std::find(bodies.begin(), bodies.end(), enemy->getRigidBody()));
    forceRegistry->removeItem(enemy->getRigidBody(), gravityForceGenerator.get());
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
    return (real) min + (rand() % (max-min+1));
}


void GameMap::removePlayer() {
    if (player != nullptr) {
        bodies.erase(std::find(bodies.begin(), bodies.end(), this->player->getRigidBody()));
         forceRegistry->removeItem(this->player->getRigidBody(), gravityForceGenerator.get());
    }
    player.reset();
}

void GameMap::setPlayer(std::shared_ptr<Player> player) {
    this->player = player;
    entities.push_back(this->player.get());
    bodies.push_back(this->player->getRigidBody());
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
    // TODO: check death of entity in another place to use only RigidBody objects for collision detection
    for (auto entity : entities) {
        // checking if enemy is dead
        if (entity->isDead()) {
            // remove it from currentEnemies and unregistering it from subject's observers
            removeEnemy(dynamic_cast<NPCEnemy *>(entity));
        }
    }
    // player not in entities
    // checking collision of entity in a body and not the other way around
//    for (int i = 0; i < 64; i++) {
//
//    }
    for (auto entity : entities) {
        // player is in bodies
        for (auto &body : bodies) {
            if (body == entity->getRigidBody()) continue;
            physics::resolveCollisions(entity->getRigidBody(), body, dt);
            if (entity != player.get()) entity->update(dt); // if entity is not the player and it is still alive
        }
    }
//    std::cout << "(" << physics::Polygon::BOTTOM_END_SCREEN.getPosition().x << ", " << physics::Polygon::BOTTOM_END_SCREEN.getPosition().y << ")" << std::endl;
}
