#ifndef Projectile_hpp
#define Projectile_hpp

#include <stdio.h>
#include "SFML/Graphics.hpp"
#include "../include/Constants.h"

enum class ProjectileType { FIRE, ICE, LIGHTNING };

class Projectile {
private:
    real velocity;
    ProjectileType type;
    sf::Vector2f position;
    sf::Sprite *sprite = nullptr;
    
public:
    Projectile();
    Projectile(real v, real x, real y, ProjectileType type);
    ~Projectile() = default;
    
    real getVelocity() {
        return velocity;
    }
    
    ProjectileType getType() {
        return type;
    }
    
    sf::Vector2f getPosition() {
        return position;
    }
    
    sf::Sprite* getSprite() {
        return sprite;
    }

    sf::IntRect getRectangle();
    
    void setVelocity(real v) {
        this->velocity = v;
    }
    
    void setPosition(real x, real y) {
        position.x = x;
        position.y = y;
    }
    
    void setProjectileType(ProjectileType type) {
        this->type = type;
    }
};

#endif /* Projectile_hpp */
