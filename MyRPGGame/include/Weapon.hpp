//#pragma once

#ifndef Weapon_hpp
#define Weapon_hpp

#include <iostream>
#include <map>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Circle.hpp"

//using namespace std;

class Weapon {
private:
    std::string name;
    int attackPoints;
    real hitRadius;
    int numHitsPerSecond;

    MoveDirection transitionDirection; // copying the entity's direction (simply for choosing the appropriate rows in the png file)
    std::map<MoveDirection, int> transitionDirectionsSpritesMap; // for defining movement and animation for the pngs
    sf::Vector2f position; // for collision detection
    sf::Texture texture;
    sf::Sprite *sprite;
    sf::IntRect spriteRect; // for setting the relevant image from the png
    int transition = Constants::WEAPON_TRANSITION_MIN;

    Circle *weaponCircle = nullptr;
    
public:
    Weapon(Point *center, WeaponType type);
    ~Weapon() = default;
    
    int getAttackPoints();
    int getNumHitsPerSecond();
    real getHitRadius();

    MoveDirection getTransitionDirection();
    std::map<MoveDirection, int> getTransitionDirectionsSpritesMap();
    sf::Vector2f getPosition();
    sf::Sprite *getSprite();
    Circle *getWeaponCircle();
    int getTransition();
    void incrementTransition();

    void setTransitionDirection(MoveDirection direction);
    
    void increaseAttackPoints(int amount);
    void increaseHitRadius(real amount);
    void increaseNumHitsPerSecond(int amount);
    void decreaseAttackPoints(int amount);
    void decreaseHitRadius(real amount);
    void decreaseNumHitsPerSecond(int amount);
    // call this function inside the battleMovementHandler
    void setIntRectPosition(int left, int top, int width, int height);
//    void attack();

    void update(Point ***point);
};

#endif /* Weapon_hpp */
