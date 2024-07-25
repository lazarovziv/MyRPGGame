#include "../include/GameEntity.hpp"

GameEntity::GameEntity() {
    level = 1;
    max_health_points = 50;
    current_health_points = max_health_points;
    max_mana_points = 30;
    current_mana_points = max_mana_points;
    attack_points = 1;
    defence_points = 5;
    current_defence_points = defence_points;
    speed = Constants::BASE_ENTITY_SPEED;
    in_battle = false;
    dead = false;
    move_direction = MoveDirection::RIGHT;
//    sprite = new sf::Sprite();
    sprite = std::make_unique<sf::Sprite>();

//    moveDirectionsSpritesMap[MoveDirection::DOWN] = 10; // change to 3
    move_directions_sprites_map[MoveDirection::DOWN] = 3;
//    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 11; // change to 4
    move_directions_sprites_map[MoveDirection::RIGHT] = 4;
//    moveDirectionsSpritesMap[MoveDirection::LEFT] = 9; // change to 2
    move_directions_sprites_map[MoveDirection::LEFT] = 2;
//    moveDirectionsSpritesMap[MoveDirection::UP] = 8; // change to 1
    move_directions_sprites_map[MoveDirection::UP] = 1;

    movement_state_row_map[EntityMovementState::IDLE] = Constants::IDLE_ROW;
    movement_state_row_map[EntityMovementState::CLIMB] = Constants::CLIMB_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = Constants::COMBAT_BACKSLASH_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = Constants::COMBAT_HALFSLASH_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = Constants::COMBAT_IDLE_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = Constants::COMBAT_SLASH_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::WALK] = Constants::WALK_ROW;
    movement_state_row_map[EntityMovementState::JUMP] = Constants::JUMP_ROW;
    movement_state_row_map[EntityMovementState::SITTING] = Constants::SITTING_ROW;
    movement_state_row_map[EntityMovementState::RUN] = Constants::RUN_ROW;
}

GameEntity::GameEntity(physics::Vector initialPosition, physics::RigidBodyType rigidBodyType,
                       const std::vector<physics::Vector> &vertices, real mass) {
    level = 1;
    max_health_points = 50;
    current_health_points = max_health_points;
    max_mana_points = 30;
    current_mana_points = max_mana_points;
    attack_points = 1;
    defence_points = 5;
    current_defence_points = defence_points;
    max_stamina_points = 35;
    current_stamina_points = max_stamina_points;
    speed = Constants::BASE_ENTITY_SPEED;
    in_battle = false;
    dead = false;
    move_direction = MoveDirection::DOWN;
    movement_state = EntityMovementState::WALK;
//    sprite = new sf::Sprite();
    sprite = std::make_unique<sf::Sprite>();

    //    moveDirectionsSpritesMap[MoveDirection::DOWN] = 10; // change to 3
    move_directions_sprites_map[MoveDirection::DOWN] = 3;
//    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 11; // change to 4
    move_directions_sprites_map[MoveDirection::RIGHT] = 4;
//    moveDirectionsSpritesMap[MoveDirection::LEFT] = 9; // change to 2
    move_directions_sprites_map[MoveDirection::LEFT] = 2;
//    moveDirectionsSpritesMap[MoveDirection::UP] = 8; // change to 1
    move_directions_sprites_map[MoveDirection::UP] = 1;

    movement_state_row_map[EntityMovementState::IDLE] = Constants::IDLE_ROW;
    movement_state_row_map[EntityMovementState::CLIMB] = Constants::CLIMB_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = Constants::COMBAT_BACKSLASH_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = Constants::COMBAT_HALFSLASH_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = Constants::COMBAT_IDLE_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = Constants::COMBAT_SLASH_ONE_HANDED_ROW;
    movement_state_row_map[EntityMovementState::WALK] = Constants::WALK_ROW;
    movement_state_row_map[EntityMovementState::JUMP] = Constants::JUMP_ROW;
    movement_state_row_map[EntityMovementState::SITTING] = Constants::SITTING_ROW;
    movement_state_row_map[EntityMovementState::RUN] = Constants::RUN_ROW;

    movement_state_col_map[EntityMovementState::IDLE] = 0;
    movement_state_col_map[EntityMovementState::CLIMB] = 0;
    movement_state_col_map[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = 0;
    movement_state_col_map[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = 0;
    movement_state_col_map[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = 0;
    movement_state_col_map[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = 0;
    movement_state_col_map[EntityMovementState::WALK] = 0;
    movement_state_col_map[EntityMovementState::JUMP] = 0;
    movement_state_col_map[EntityMovementState::SITTING] = 0;
    movement_state_col_map[EntityMovementState::RUN] = 0;

    switch (rigidBodyType) {
        case physics::RigidBodyType::CIRCLE: {
            rigid_body = std::make_unique<physics::Circle>(initialPosition.x, initialPosition.y, initialPosition.z,
                                                          (real) Constants::TILE_SIZE/4);
            break;
        }
        case physics::RigidBodyType::POLYGON: {
            rigid_body = std::make_unique<physics::Polygon>(initialPosition.x, initialPosition.y, initialPosition.z,
                                                           vertices, mass);
            break;
        }
        default:
            break;
    }
    // updating the mass because derived classes aren't setting new values to the mass in their constructors
    rigid_body->set_mass(mass);

    position.x = rigid_body->get_position().x;
    position.y = rigid_body->get_position().y;
}

GameEntity::~GameEntity() {
//    delete sprite;
}

void GameEntity::increate_level(int amount) {
    level += amount;
}

void GameEntity::increase_max_health_points(int amount) {
    max_health_points += amount;
    // also adding amount to current health points
    if (current_health_points + amount >= max_health_points) {
        current_health_points = max_health_points;
    } else current_health_points += amount;
}

void GameEntity::increase_max_mana_points(int amount) {
    max_mana_points += amount;
    // same like increaseMaxHealthPoints
    if (current_mana_points + amount >= max_mana_points) {
        current_mana_points = max_mana_points;
    } else current_mana_points += amount;
}

void GameEntity::increase_speed(const real amount) {
    // setting upper limit to 3
//    if (speed + amount < 3) {
//        speed += amount;
//    }
    speed += amount;
}

void GameEntity::set_speed(real newSpeed) {
    speed = newSpeed;
}

void GameEntity::increase_attack_points(const int amount) {
    attack_points += amount;
}

void GameEntity::increase_defence_points(const int amount) {
    defence_points += amount;
    // increasing current defence points altogether
    current_defence_points += amount;
}

void GameEntity::change_in_battle_state() {
    in_battle = !in_battle;
}

void GameEntity::set_move_direction(const MoveDirection direction) {
    move_direction = direction;
    // adjusting weapon direction
//    weapon->setTransitionDirection(direction);
}

void GameEntity::set_movement_state(const EntityMovementState state) {
    movement_state = state;
}

void GameEntity::increment_step() {
    if (step < Constants::NUM_STEPS) step++;
    else step = 0;
}

void GameEntity::set_x(const real x) {
    position.x = x;
}

void GameEntity::set_y(const real y) {
    position.y = y;
}

// TODO: add direction vector and normalize it for diagonal movement?
void GameEntity::set_position(const real x, const real y, const real z) {
    rigid_body->set_position(x, y, z);
}

void GameEntity::set_position(const physics::Vector &newPosition) {
    rigid_body->set_position(newPosition.x, newPosition.y, newPosition.z);
}

void GameEntity::move(const physics::Vector &directionVector, const real dt) {
    // TODO: delete this after anti gravity force will be added
    if (directionVector == physics::Vector::ZERO) {
        position_updated = false;
        running = false;
        moving = false;
        return;
    }
    real currentSpeed = running ? speed * 2.f : speed; // multiply by dt?
    moving = true;

    rigid_body->add_force(directionVector * currentSpeed);
    // incrementing the distance traveled a bit lower than it should be when running to look realistic when animating
    increment_distance_traveled_since_idle(running ? ((real) 2/3) * currentSpeed * dt : currentSpeed * dt);
    // setting the moveDirection used for animating
    set_move_direction(directionVector);
    position_updated = true;
    // TODO: add reset to all entity movement states columns values besides the relevant state that'll be used
    // OR reset only the last state the player made
    for (auto &state : Constants::COMBAT_STATES) movement_state_col_map[state] = 0;
}

bool GameEntity::jump(const physics::Vector &directionVector, const real dt) {
    // don't apply force if interval hasn't been reset yet
    if (jump_interval < 0) {
        std::cout << jump_interval << std::endl;
        return false;
    }
    real currentJumpScaler = running ? speed * 2 : speed;
    // TODO: make the force to be applied when the animation finishes
    physics::Vector jumpDirectionVector = physics::Vector::ZERO;
    if (directionVector == physics::Vector::ZERO) {
        // setting the force direction based on the move direction of the jump
        switch (move_direction) {
            case MoveDirection::UP: {
                jumpDirectionVector = physics::Vector::UP_DIRECTION;
                // scaling the y value if entity means to jump vertically
                jumpDirectionVector.y *= currentJumpScaler;
                break;
            }
            case MoveDirection::DOWN: {
                jumpDirectionVector = physics::Vector::DOWN_DIRECTION;
                // scaling the y value if entity means to jump vertically
                jumpDirectionVector.y *= currentJumpScaler;
                break;
            }
            case MoveDirection::RIGHT: {
                jumpDirectionVector = physics::Vector::RIGHT_DIRECTION;
                // scaling the y value if entity means to jump horizontally
                jumpDirectionVector.x *= currentJumpScaler;
                break;
            }
            case MoveDirection::LEFT: {
                jumpDirectionVector = physics::Vector::LEFT_DIRECTION;
                // scaling the y value if entity means to jump horizontally
                jumpDirectionVector.x *= currentJumpScaler;
                break;
            }
            default: {
                jumpDirectionVector = physics::Vector::ZERO;
            }
        }
    }
    // incrementing the z axis for using high floors (to be implemented later)
    jumpDirectionVector.z += currentJumpScaler;
    // applying the force (should be applied when animation is about to finish)
    rigid_body->add_force(jumpDirectionVector);
    increment_jump_height_since_on_ground(currentJumpScaler * dt);
    set_move_direction(directionVector);

    return true;
}

void GameEntity::set_move_direction(const physics::Vector directionVector) {
    real horizontalDirection = directionVector.x;
    real verticalDirection = directionVector.y;
    if (horizontalDirection > 0) move_direction = MoveDirection::RIGHT;
    else if (horizontalDirection < 0) move_direction = MoveDirection::LEFT;
    else {
        if (verticalDirection > 0) move_direction = MoveDirection::DOWN;
        else if (verticalDirection < 0) move_direction = MoveDirection::UP;
    }
}

void GameEntity::set_weapon(const WeaponType type) {
    weapon = std::make_unique<Weapon>(rigid_body->get_position(), type);
}

void GameEntity::set_is_in_battle(const bool flag) {
    this->in_battle = flag;
}

void GameEntity::decrease_max_health_points(const real amount) {
    // don't let it decrease (throw an error)
    if (max_health_points - amount <= 0) return;
    max_health_points -= amount; // not changing currentHealthPoints like when increasing
}

void GameEntity::decrease_current_health_points(const real amount) {
    if (current_health_points > 0) {
        if (current_health_points - amount <= 0) current_health_points = 0;
        else current_health_points -= amount;
    }
    if (current_health_points <= 0) dead = true;
}

void GameEntity::decrease_max_mana_points(const real amount) {
    if (max_mana_points - amount <= 0) max_mana_points = 0;
    max_mana_points -= amount;
}

void GameEntity::decrease_speed(const real amount) {
    if (this->speed - amount <= 0) this->speed = 0;
    this->speed -= amount;
}

void GameEntity::decrease_attack_points(const real amount) {
    if (attack_points - amount <= 0) attack_points = 0;
    attack_points -= amount;
}

void GameEntity::decrease_defence_points(const real amount) {
    if (defence_points - amount <= 0) defence_points = 0;
    defence_points -= amount;
}

void GameEntity::decrease_current_defence_points(const real amount) {
    if (current_defence_points - amount <= 0) current_defence_points = 0;
    else current_defence_points -= amount;
}

long GameEntity::get_id() const {
    return id;
}

char* GameEntity::get_name() {
    return name;
}

int GameEntity::get_level() const {
    return level;
}

real GameEntity::get_max_health_points() const {
    return max_health_points;
}

real GameEntity::get_current_health_points() const {
    return current_health_points;
}

real GameEntity::get_max_mana_points() const {
    return max_mana_points;
}

real GameEntity::get_current_mana_points() const {
    return current_mana_points;
}

real GameEntity::get_attack_points() const {
    return attack_points;
}

real GameEntity::get_defence_points() const {
    return defence_points;
}

real GameEntity::get_current_defence_points() const {
    return current_defence_points;
}

real GameEntity::get_max_stamina_points() const {
    return max_stamina_points;
}

real GameEntity::get_current_stamina_points() const {
    return current_stamina_points;
}

real GameEntity::get_speed() const {
    return speed;
}

int GameEntity::get_step() const {
    return step;
}

bool GameEntity::is_in_battle() const {
    return in_battle;
}

bool GameEntity::is_dead() const {
    return dead;
}

MoveDirection GameEntity::get_move_direction() const {
    return move_direction;
}

EntityMovementState GameEntity::get_movement_state() const {
    return movement_state;
}

std::map<MoveDirection, int> GameEntity::get_move_directions_sprites_map() const {
    return move_directions_sprites_map;
}

int GameEntity::get_movement_state_col_count(const EntityMovementState state) const {
    return movement_state_col_map.at(state);
}

void GameEntity::increment_movement_state_col_count(const EntityMovementState state) {
    if (movement_state_col_map[state] < Constants::MOVEMENT_STATE_NUM_COLS.at(state)-1) {
        movement_state_col_map[state] = movement_state_col_map[state] + 1;
        return;
    }
    movement_state_col_map[state] = 0;
}

void GameEntity::reset_movement_state_col_count(const EntityMovementState state) {
    movement_state_col_map[state] = 0;
}

std::map<EntityMovementState, int> GameEntity::get_movement_state_row_map() {
    return movement_state_row_map;
}

sf::Vector2f GameEntity::get_render_position() const {
    return position;
}

sf::Sprite* GameEntity::get_sprite() const {
    return sprite.get();
}

void GameEntity::set_sprite(sf::Sprite *newSprite) {
//    sprite = newSprite;
}

sf::Sprite* GameEntity::get_movement_state_sprite(const EntityMovementState state) {
    return movement_state_sprites_map[state];
}

sf::IntRect GameEntity::get_rectangle() const {
    return (sf::IntRect) sprite->getGlobalBounds();
}

void GameEntity::set_int_rect_position(const int left, const int top, const int width, const int height) {
    sprite_rect.left = left;
    sprite_rect.top = top;
    sprite_rect.width = width;
    sprite_rect.height = height;
    sprite->setTextureRect(sprite_rect);
}

physics::RigidBody* GameEntity::get_rigid_body() const {
    return rigid_body.get();
}

physics::Vector &GameEntity::get_position() const {
    return (*rigid_body).get_position();
}

bool GameEntity::is_running() const {
    return running;
}

void GameEntity::set_is_running(const bool flag) {
    running = flag;
}

bool GameEntity::is_moving() const {
    return moving;
}

void GameEntity::set_is_moving(const bool flag) {
    moving = flag;
}

void GameEntity::reset_moving() {
    moving = false;
    running = false;
}

bool GameEntity::is_attacking() const {
    return attacking;
}

void GameEntity::set_is_attacking(const bool flag) {
    attacking = flag;
}

void GameEntity::reset_attacking() {
    reset_battle_interval_for_swing();
    attacking = false;
}

bool GameEntity::is_jumping() const {
    return jumping;
}

void GameEntity::set_is_jumping(const bool flag) {
    jumping = flag;
}

void GameEntity::reset_jumping() {
    reset_jump_interval();
    jumping = false;
}

bool GameEntity::can_attack() const {
    return battle_interval >= BATTLE_INTERVAL_DEFAULT && !moving;
}

void GameEntity::reset_battle_interval() {
    battle_interval = 0;
}

void GameEntity::increase_battle_interval(const real dt) {
    battle_interval += dt;
}

void GameEntity::reset_battle_interval_for_swing() {
    battle_interval = -SWING_INTERVAL_DEFAULT;
}

bool GameEntity::did_just_move() const {
    return just_moved;
}

void GameEntity::set_just_moved(const bool flag) {
    just_moved = flag;
}

// direction is chosen randomly
bool GameEntity::can_go_idle() const {
    return move_interval >= MOVE_INTERVAL_DEFAULT;
}

bool GameEntity::can_change_direction() const {
    return change_move_direction_interval >= CHANGE_MOVE_DIRECTION_INTERVAL;
}

void GameEntity::reset_change_direction_interval() {
    change_move_direction_interval = 0;
}

void GameEntity::reset_move_interval() {
    move_interval = 0;
    reset_idle_animation_interval();
}

void GameEntity::set_is_idle(const bool flag) {
    idle = flag;
}

bool GameEntity::is_idle() const {
    return idle;
}

void GameEntity::reset_distance_traveled_since_idle() {
    distance_traveled_since_idle = 0;
}

void GameEntity::increment_distance_traveled_since_idle(const real distance) {
    distance_traveled_since_idle += distance;
}

void GameEntity::increment_jump_height_since_on_ground(const real distance) {
    jump_interval += distance;
}

bool GameEntity::can_animate_movement(const bool check) {
    // checking we're not animating other animation type
    if (jumping || attacking) return false;
    if (distance_traveled_since_idle >= speed) {
        if (!check) reset_distance_traveled_since_idle();
        return true;
    }
    return false;
}

bool GameEntity::can_animate_idle(const bool check) {
    // checking we're not animating other animation type
    if (jumping || attacking || moving) return false;
    if (idle_animation_interval >= Constants::NUM_FRAMES_IDLE_ANIMATION) {
        if (!check) reset_idle_animation_interval();
        return true;
    }
    return false;
}

bool GameEntity::can_animate_jump(const bool check) {
    // checking we're not animating other animation type
    if (moving || attacking) return false;
    if (jump_interval >= JUMP_HEIGHT_INTERVAL_DEFAULT) {
        if (!check) reset_jump_height_since_on_ground_interval();
        return true;
    }
    return false;
}

void GameEntity::reset_idle_animation_interval() {
    idle_animation_interval = 0;
//    resetMovementStateColCount(EntityMovementState::COMBAT_SLASH_ONE_HANDED);
//    resetMovementStateColCount(EntityMovementState::WALK);
}

void GameEntity::reset_jump_height_since_on_ground_interval() {
    jump_interval = 0;
}

void GameEntity::reset_jump_interval() {
    jump_interval = -JUMP_INTERVAL_DEFAULT;
//    jumpHeightSinceOnGround = 0;
}

void GameEntity::increment_idle_animation_interval(const real dt) {
    idle_animation_interval += dt;
}

Weapon* GameEntity::get_weapon() {
    return weapon.get();
}

void GameEntity::print_position() const {
    rigid_body->get_position().print_coordinates();
}

void GameEntity::update(const real dt) {
    if (!dead) {
        rigid_body->update(dt);

        position.x = rigid_body->get_position().x;
        position.y = rigid_body->get_position().y;
        sprite->setPosition(position.x, position.y);
        // updating intervals (checking values to prevent overflow if not acted long enough)
        move_interval += dt;
        battle_interval += dt;
        jump_interval += dt;
        if (!is_player) change_move_direction_interval += dt;
    }
}
