//#pragma once

#ifndef GameEntity_hpp
#define GameEntity_hpp

#include <cmath>
#include <map>
#include <stack>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Weapon.hpp"
#include "Circle.hpp"
#include "RigidBody.hpp"

//#include "GameEntityMovement.hpp"
//class GameEntityMovement;
class GameEntityMovement;

const int MALE_GENDER = 0;
const int FEMALE_GENDER = 1;

class GameEntity {
protected:
    long id;
    char name[10];
    int level;
    int gender; // 0 for male, 1 for female
    real current_health_points;
    real max_health_points;
    real current_mana_points;
    real max_mana_points;
    real attack_points;
    real defence_points;
    real current_defence_points;
    real speed;
    real max_stamina_points;
    real current_stamina_points;

    bool is_player = false;
    bool is_landscape = false;
    // TODO: add flag for indicating whether the entity is on the ground
    // flag for idle state and time delta incrementation
    bool position_updated = false;

    // counter for sprite change (0 to 3)
    int step = 0;
    bool in_battle = false;
    bool dead = false;
    bool moving = true;
    bool running = false;
    bool attacking = false;
    bool jumping = false;
    MoveDirection move_direction;
    EntityMovementState movement_state;
    std::map<MoveDirection, int> move_directions_sprites_map;
    std::map<EntityMovementState, int> movement_state_row_map;
    std::map<EntityMovementState, int> movement_state_col_map;
//    EntityMovementState lastCombatStateUsed;
    // field for all collisions and position of the entity
    std::unique_ptr<physics::RigidBody> rigid_body;

    sf::Vector2f position;
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
//    sf::Sprite *sprite; // maybe VertexArray for each direction
    // for handling changing in entity's movement (from walking to running, from idle to combat etc.)
    std::vector<sf::Texture> movement_state_textures;
    std::map<EntityMovementState, sf::Sprite*> movement_state_sprites_map;
    sf::IntRect sprite_rect;
//    GameMap* currentGameMap;
    std::unique_ptr<Weapon> weapon;

    // combat intervals between each frame in the swing
    constexpr static const real BATTLE_INTERVAL_DEFAULT = (real) 0.75f;
    real battle_interval = 0.f;
    bool just_moved = false;

    // interval between swings
    constexpr static const real SWING_INTERVAL_DEFAULT = (real) 8.0f;

    // change direction interval (for enemies only!)
    constexpr static const real CHANGE_MOVE_DIRECTION_INTERVAL = 256.0f;
    real change_move_direction_interval = 0;

    // movement intervals
    constexpr static const real MOVE_INTERVAL_DEFAULT = 3.0f;
    real move_interval = 0.f;
    bool idle = true;
    // for the animations. after traveled the speed distance, increment relevant animation count
    real distance_traveled_since_idle = 0;
    // idle interval
    real idle_animation_interval = 0;

    constexpr static const real JUMP_HEIGHT_INTERVAL_DEFAULT = 4.0f;
    // for between jumps
    constexpr static const real JUMP_INTERVAL_DEFAULT = 8.0f;
    real jump_interval = 0;

    std::stack<Point *> moves_stack;
    
private:
//    GameEntityMovement* movement;
    
public:
    GameEntity();
    explicit GameEntity(physics::Vector initial_position, physics::RigidBodyType rigid_body_type,
                        const std::vector<physics::Vector> &vertices = {}, real mass = 1);
    virtual ~GameEntity();
    // getters
    long get_id() const;
    char* get_name();
    int get_level() const;
    int get_gender() const;
    real get_max_health_points() const;
    real get_current_health_points() const;
    real get_max_mana_points() const;
    real get_current_mana_points() const;
    real get_attack_points() const;
    real get_defence_points() const;
    real get_current_defence_points() const;
    real get_max_stamina_points() const;
    real get_current_stamina_points() const;
    real get_speed() const;
    int get_step() const;
    bool is_in_battle() const;
    bool is_dead() const;
    MoveDirection get_move_direction() const;
    EntityMovementState get_movement_state() const;
    std::map<MoveDirection, int> get_move_directions_sprites_map() const;
    int get_movement_state_col_count(EntityMovementState state) const;
    void increment_movement_state_col_count(EntityMovementState state);
    void reset_movement_state_col_count(EntityMovementState state);
    std::map<EntityMovementState, int> get_movement_state_row_map();
    sf::Vector2f get_render_position() const;
    sf::Sprite* get_sprite() const;
    sf::IntRect get_rectangle() const; // sprite.getGlobalBounds()
    void set_int_rect_position(int left, int top, int width, int height);

    physics::RigidBody *get_rigid_body() const;
    physics::Vector &get_position() const;
    
    void increate_level(int amount);
    void increase_max_health_points(int amount);
    void increase_max_mana_points(int amount);
    void increase_speed(const real amount);
    void set_speed(real new_speed);
    void increase_attack_points(int amount);
    void increase_defence_points(int amount);
    void change_in_battle_state();
    void set_move_direction(MoveDirection direction);
    void set_movement_state(EntityMovementState state);
    void increment_step();
    void set_x(real x);
    void set_y(real y);
    void set_position(real x, real y, real z = 0);
    void set_position(const physics::Vector &new_position);
    void move(const physics::Vector &direction_vector, real dt);
    bool jump(const physics::Vector &direction_vector, real dt);
    void set_move_direction(physics::Vector direction_vector);

    bool create_movement_state_sprite(EntityMovementState state);
    bool add_movement_state_sprite(EntityMovementState state, sf::Sprite *new_sprite); // if sprite is null, we'll create one based on the state
    void set_weapon(WeaponType type);
    void set_is_in_battle(bool in_battle);

    // for situations where changing for worse equipment (adding logic for negative base values)
    void decrease_max_health_points(real amount);
    void decrease_max_mana_points(real amount);
    void decrease_current_health_points(real amount);
    void decrease_current_mana_points(real amount);
    void decrease_speed(real speed);
    void decrease_attack_points(real amount);
    void decrease_defence_points(real amount);
    void decrease_current_defence_points(real amount);

    sf::Sprite* get_movement_state_sprite(EntityMovementState state);
    void set_sprite(sf::Sprite *new_sprite);
    Weapon *get_weapon();

    bool is_running() const;
    void set_is_running(bool flag);
    bool is_moving() const;
    void set_is_moving(bool flag);
    void reset_moving();

    bool is_attacking() const;
    void set_is_attacking(bool flag);
    void reset_attacking();

    bool is_jumping() const;
    void set_is_jumping(bool flag);
    void reset_jumping();

    bool can_attack() const;
    void reset_battle_interval();
    void increase_battle_interval(real dt);
    void reset_battle_interval_for_swing();
    bool did_just_move() const;
    void set_just_moved(bool flag);
    bool is_idle() const;
    void reset_distance_traveled_since_idle();
    void increment_distance_traveled_since_idle(real distance);
    void increment_jump_height_since_on_ground(real distance);
    bool can_animate_movement(bool check = false);

    bool can_animate_idle(bool check = false);
    void reset_idle_animation_interval();
    void reset_jump_height_since_on_ground_interval();
    void reset_jump_interval();
    void increment_idle_animation_interval(real dt);

    bool can_animate_jump(bool check = false);

    bool can_go_idle() const;
    bool can_change_direction() const;
    void reset_change_direction_interval();
    void reset_move_interval();
    void set_is_idle(bool flag);

    void print_position() const;
    
    void update(real dt);
};

#endif /* GameEntity_hpp */
