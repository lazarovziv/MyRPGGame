//#pragma once

#ifndef Weapon_hpp
#define Weapon_hpp

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Circle.hpp"
#include "RigidBody.hpp"

//using namespace std;

class Weapon {
private:
    std::string name;
    int attack_points;
    real hit_radius;
    int num_hits_per_second;

    MoveDirection transition_direction; // copying the entity's direction (simply for choosing the appropriate rows in the png file)
    std::map<MoveDirection, int> transition_directions_sprites_map; // for defining movement and animation for the pngs
    sf::Vector2f position; // for collision detection
    sf::Texture texture;
    sf::Sprite *sprite;
    sf::IntRect sprite_rect; // for setting the relevant image from the png
    int transition = Constants::WEAPON_TRANSITION_MIN;

    Circle *weapon_circle = nullptr;
    std::unique_ptr<physics::RigidBody> rigid_body;
    
public:
    Weapon(physics::Vector initial_position, WeaponType type);
    ~Weapon() = default;
    
    int get_attack_points();
    int get_num_hits_per_second();
    real get_hit_radius();

    MoveDirection get_transition_direction();
    std::map<MoveDirection, int> get_transition_directions_sprites_map();
    sf::Vector2f get_position();
    sf::Sprite *get_sprite();
    Circle *get_weapon_circle();
    int get_transition();
    void increment_transition();

    void set_transition_direction(MoveDirection direction);
    
    void increase_attack_points(int amount);
    void increase_hit_radius(real amount);
    void increase_num_hits_per_second(int amount);
    void decrease_attack_points(int amount);
    void decrease_hit_radius(real amount);
    void decrease_num_hits_per_second(int amount);
    // call this function inside the battleMovementHandler
    void set_int_rect_position(int left, int top, int width, int height);
//    void attack();

    void update(real dt);
};

#endif /* Weapon_hpp */
