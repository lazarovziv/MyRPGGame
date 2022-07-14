//#pragma once

#ifndef Weapon_hpp
#define Weapon_hpp

#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

enum class WeaponType { SWORD, DAGGER, STAFF, AXE, MACE, BARE_HANDED };

class Weapon {
private:
    string name;
    int attackPoints;
    float hitRadius;
    int numHitsPerSecond;
    Sprite sprite;
    
public:
    Weapon(WeaponType type);
    ~Weapon() = default;
    
    int getAttackPoints();
    int getNumHitsPerSecond();
    float getHitRadius();
    
    void increaseAttackPoints(int amount);
    void increaseHitRadius(float amount);
    void increaseNumHitsPerSecond(int amount);
    void decreaseAttackPoints(int amount);
    void decreaseHitRadius(float amount);
    void decreaseNumHitsPerSecond(int amount);
//    void attack();
};

#endif /* Weapon_hpp */
