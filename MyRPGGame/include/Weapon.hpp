//#pragma once

#ifndef Weapon_hpp
#define Weapon_hpp

#include <iostream>
#include <map>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Circle.hpp"

using namespace sf;
using namespace std;

class Weapon {
private:
    string name;
    int attackPoints;
    float hitRadius;
    int numHitsPerSecond;

    MoveDirection transitionDirection; // copying the entity's direction (simply for choosing the appropriate rows in the png file)
    map<MoveDirection, int> transitionDirectionsSpritesMap; // for defining movement and animation for the pngs
    Vector2i position; // for collision detection
    Texture texture;
    Sprite *sprite;
    IntRect spriteRect; // for setting the relevant image from the png
    int transition = Constants::WEAPON_TRANSITION_MIN;

    Circle *weaponCircle = nullptr;
    
public:
    Weapon(Point *center, WeaponType type);
    ~Weapon() = default;
    
    int getAttackPoints();
    int getNumHitsPerSecond();
    float getHitRadius();

    MoveDirection getTransitionDirection();
    map<MoveDirection, int> getTransitionDirectionsSpritesMap();
    Vector2i getPosition();
    Sprite *getSprite();
    Circle *getWeaponCircle();
    int getTransition();
    void incrementTransition();

    void setTransitionDirection(MoveDirection direction);
    
    void increaseAttackPoints(int amount);
    void increaseHitRadius(float amount);
    void increaseNumHitsPerSecond(int amount);
    void decreaseAttackPoints(int amount);
    void decreaseHitRadius(float amount);
    void decreaseNumHitsPerSecond(int amount);
    // call this function inside the battleMovementHandler
    void setIntRectPosition(int left, int top, int width, int height);
//    void attack();

    void update(Point ***point);
};

#endif /* Weapon_hpp */
