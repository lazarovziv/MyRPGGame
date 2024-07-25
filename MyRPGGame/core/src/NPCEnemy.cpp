#include "../include/NPCEnemy.hpp"

NPCEnemy::NPCEnemy(int type, physics::Vector initialPosition) : GameEntity(initialPosition, physics::RigidBodyType::CIRCLE) {
    this->type = type;
    level = 1;
    max_health_points = 350000;
    current_health_points = max_health_points;
    max_mana_points = 10;
    current_mana_points = max_mana_points;
    attack_points = 2;
    defence_points = 1;
    current_defence_points = defence_points;
    exp_points_worth = 10;
    in_battle = false;
    dead = false;
    speed = Constants::BASE_ENTITY_SPEED;
    move_direction = MoveDirection::UP;

    // TextureLoader.getInstance()->loadTexture("dorio_64.png");
    texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "enemies/green_orcs/spritesheet.png");
    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(move_directions_sprites_map[move_direction] * Constants::TILE_SIZE,
                                       Constants::WALK_ROW * Constants::TILE_SIZE,
                                       Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
    move_direction = MoveDirection::DOWN;

    rigid_body->set_mass(5);
    // TODO: choose random floats in defined location radius for each enemy in map
}

NPCEnemy::~NPCEnemy() {
//    delete weapon;
}

int NPCEnemy::get_battle_timeout() {
    return battle_interval;
}

real NPCEnemy::get_wander_area_radius() {
    return wander_area_radius;
}

real NPCEnemy::get_battle_area_radius() {
    return battle_area_radius;
}

int NPCEnemy::get_exp_points_worth() {
    return exp_points_worth;
}

int NPCEnemy::get_type() {
    return type;
}

bool NPCEnemy::can_move() const {
    return move_interval >= GameEntity::MOVE_INTERVAL_DEFAULT;
}

bool NPCEnemy::can_go_to_wander_area() {
    return wander_area_interval >= WANDER_AREA_INTERVAL_DEFAULT;
}

bool NPCEnemy::is_in_battle_area() {
    if (battle_area_circle == nullptr) return false;
//    return battleAreaCircle->isPointInCircle(entityCircle->getCenter());
    return false;
}

bool NPCEnemy::is_in_wander_area() {
//    if (wanderAreaCircle == nullptr) return false;
//    return wanderAreaCircle->isPointInCircle(entityCircle->getCenter());
    return true;
}

Circle *NPCEnemy::get_wander_area_circle() {
    return wander_area_circle;
}

Circle *NPCEnemy::get_battle_area_circle() {
    return battle_area_circle;
}

void NPCEnemy::set_move_interval(real interval) {
    move_interval = interval;
}

// TODO: what to do here? how to use observers to my advantage
void NPCEnemy::notify() {
    // updating battle state
    // enemy chased player and went out of battle area
    if (in_battle && !is_in_battle_area()) in_battle = false;
}
