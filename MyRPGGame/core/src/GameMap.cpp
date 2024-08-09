#include "GameMap.hpp"
//class NPCEnemy;

GameMap::GameMap(int row, int col) {
    world_map_row = row;
    world_map_col = col;
}

GameMap::GameMap(int row, int col, bool up, bool down, bool right, bool left) {
    world_map_row = row;
    world_map_col = col;
    exitableFromTop = up;
    exitableFromBottom = down;
    exitableFromRight = right;
    exitableFromLeft = left;
//    backgroundSprite = new sf::Sprite();
    background_sprite = std::make_unique<sf::Sprite>();

    if (texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "maps/Sample.png")) {
        std::cout << "Background loaded properly." << std::endl;
    } else std::cout << "Background NOT loaded." << std::endl;
//    texture.setSmooth(true);
    background_sprite->setTexture(texture);
    background_sprite->setOrigin(Constants::FULL_SCREEN_WIDTH/2, Constants::FULL_SCREEN_HEIGHT/2);
    background_sprite->setPosition(Constants::FULL_SCREEN_WIDTH/2, Constants::FULL_SCREEN_HEIGHT/2);

    // initializing gravity for map
    gravity_force_generator = std::make_unique<physics::RigidBodyGravity>(physics::Vector{0,  (real) 9.81});
    ground_force_generator = std::make_unique<physics::RigidBodyGravity>(physics::Vector{0, (real) -9.81});
    // gravityForceGenerator = std::make_unique<physics::RigidBodyGravity>(physics::Vector::ZERO);
    force_registry = std::make_unique<physics::RigidBodyForceRegistry>();
    // forceRegistry->addItem(&physics::Polygon::RIGHT_END_SCREEN, gravityForceGenerator.get());
    // forceRegistry->addItem(&physics::Polygon::LEFT_END_SCREEN, gravityForceGenerator.get());
    // forceRegistry->addItem(&physics::Polygon::TOP_END_SCREEN, gravityForceGenerator.get());
    // forceRegistry->addItem(&physics::Polygon::BOTTOM_END_SCREEN, gravityForceGenerator.get());
    bodies.push_back(&physics::Polygon::RIGHT_END_SCREEN);
    bodies.push_back(&physics::Polygon::LEFT_END_SCREEN);
    bodies.push_back(&physics::Polygon::TOP_END_SCREEN);
    bodies.push_back(&physics::Polygon::BOTTOM_END_SCREEN);
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

    for (auto & enemy : enemies_vector) {
        delete enemy;
    }

    delete map_graph;
}

int GameMap::get_world_map_row() const {
    return world_map_row;
}

int GameMap::get_world_map_col() const {
    return world_map_col;
}

sf::Sprite* GameMap::get_background_sprite() {
    return background_sprite.get();
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

std::vector<LandscapeEntity*> GameMap::get_landscapes() {
    return landscapes;
}

void GameMap::add_landscape(LandscapeEntity *entity) {
    landscapes.push_back(entity);
    bodies.push_back(entity->get_rigid_body());
    force_registry->add_item(entity->get_rigid_body(), gravity_force_generator.get());
    force_registry->add_item(entity->get_rigid_body(), ground_force_generator.get());
}

std::vector<NPCEnemy *> GameMap::get_enemies() {
    return enemies_vector;
}

physics::RigidBodyForceRegistry* GameMap::get_force_registry() const {
    return force_registry.get();
}

void GameMap::init() {
    // don't add more enemies
    if (enemies_vector.size() >= NUM_OF_MAX_ENEMIES) return;
    // seeding
    srand((unsigned int) time(nullptr));
    int randX = generate_random(Constants::TILE_SIZE/2, Constants::SCREEN_WIDTH - Constants::TILE_SIZE/2);
    int randY = generate_random(Constants::TILE_SIZE/2, Constants::SCREEN_HEIGHT - Constants::TILE_SIZE/2);
    // clamping the enemies spawn coordinates
    randX = ((randX/16) * 16) % (int) Constants::FULL_SCREEN_WIDTH;
    randY = ((randY/16) * 16) % (int) Constants::FULL_SCREEN_HEIGHT;

    auto *enemy = new NPCEnemy(NPCEnemy::WORM, physics::Vector{(real) randX, (real) randY});
    enemy->increase_defence_points(20);
    add_enemy(enemy);
}

void GameMap::add_enemy(NPCEnemy *enemy) {
    enemies_vector.push_back(enemy);
    entities.push_back(enemy);
    bodies.push_back(enemy->get_rigid_body());
    // TODO: make this more generic
    force_registry->add_item(enemy->get_rigid_body(), gravity_force_generator.get());
    force_registry->add_item(enemy->get_rigid_body(), ground_force_generator.get());
    // registering observer
    if (player != nullptr) player->register_observer(enemy);
}

void GameMap::remove_enemy_at_index(int i) {
//    delete enemiesVector[i];
    enemies_vector.erase(enemies_vector.begin() + i);
}

void GameMap::remove_enemy(NPCEnemy *enemy) {
    enemies_vector.erase(std::find(enemies_vector.begin(), enemies_vector.end(), enemy));
    entities.erase(std::find(entities.begin(), entities.end(), enemy));
    bodies.erase(std::find(bodies.begin(), bodies.end(), enemy->get_rigid_body()));
    force_registry->remove_item(enemy->get_rigid_body(), gravity_force_generator.get());
    force_registry->remove_item(enemy->get_rigid_body(), ground_force_generator.get());
    // unregistering enemy from player's observers
    player->unregister_observer(enemy);
}

void GameMap::remove_all_enemies() {
    for (auto &enemy : enemies_vector) {
        delete enemy;
    }
    enemies_vector.clear();
}   

real GameMap::generate_random(int min, int max) {
    return (real) min + (rand() % (max-min+1));
}


void GameMap::remove_player() {
    if (player != nullptr) {
        bodies.erase(std::find(bodies.begin(), bodies.end(), this->player->get_rigid_body()));
        force_registry->remove_item(this->player->get_rigid_body(), gravity_force_generator.get());
        force_registry->remove_item(this->player->get_rigid_body(), ground_force_generator.get());
    }
    player.reset();
}

void GameMap::set_player(std::shared_ptr<Player> player) {
    this->player = player;
    entities.push_back(this->player.get());
    bodies.push_back(this->player->get_rigid_body());
    force_registry->add_item(this->player->get_rigid_body(), gravity_force_generator.get());
    force_registry->add_item(this->player->get_rigid_body(), ground_force_generator.get());
}

Player *GameMap::get_player() {
    return player.get();
}

void GameMap::set_top_exit_circle(Circle *circle) {
    topExitCircle.reset(circle);
}

void GameMap::set_bottom_exit_circle(Circle *circle) {
    bottomExitCircle.reset(circle);
}

void GameMap::set_right_exit_circle(Circle *circle) {
    rightExitCircle.reset(circle);
}

void GameMap::set_left_exit_circle(Circle *circle) {
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
    return map.world_map_row == world_map_row && map.world_map_col == world_map_col;
}

void GameMap::resolve_collisions(const real dt) {
    for (auto entity : entities) {
        // player is in bodies
        for (auto &body : bodies) {
            if (body == entity->get_rigid_body()) continue;
            physics::resolve_collisions(entity->get_rigid_body(), body, dt);
        }
    }
}

void GameMap::update(const real dt) {
    for (auto entity : entities) {
        // checking if enemy is dead
        if (entity->is_dead()) {
            // remove it from currentEnemies and unregistering it from subject's observers
            remove_enemy(dynamic_cast<NPCEnemy *>(entity));
        }
    }
    // player not in entities
    // checking collision of entity in a body and not the other way around
    real divDt = dt/Constants::UPDATE_ITERATIONS;
    for (auto entity : entities) {
        // player is in bodies
        entity->update(divDt);
    }

    // updating physics
    force_registry->update(dt);
}
