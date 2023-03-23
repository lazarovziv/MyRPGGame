#ifndef Projectile_hpp
#define Projectile_hpp

#include <stdio.h>
#include "SFML/Graphics.hpp"
#include "../include/Constants.h"

using namespace sf;

enum class ProjectileType { FIRE, ICE, LIGHTNING };

class Projectile {
private:
    int velocity;
    ProjectileType type;
    Vector2i position;
    Sprite *sprite = nullptr;
    
public:
    Projectile();
    Projectile(int v, int x, int y, ProjectileType type);
    ~Projectile() = default;
    
    int getVelocity() {
        return velocity;
    }
    
    ProjectileType getType() {
        return type;
    }
    
    Vector2i getPosition() {
        return position;
    }
    
    Sprite* getSprite() {
        return sprite;
    }

    IntRect getRectangle();
    
    void setVelocity(int v) {
        this->velocity = v;
    }
    
    void setPosition(int x, int y) {
        position.x = x;
        position.y = y;
    }
    
    void setProjectileType(ProjectileType type) {
        this->type = type;
    }
};

#endif /* Projectile_hpp */
