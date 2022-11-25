#ifndef Projectile_hpp
#define Projectile_hpp

#include <stdio.h>
#include "SFML/Graphics.hpp"

using namespace sf;

enum class ProjectileType { FIRE, ICE, LIGHTNING };

class Projectile {
private:
    float velocity;
    ProjectileType type;
    Vector2f position;
    Sprite* sprite = nullptr;
    
public:
    Projectile();
    Projectile(float v, float x, float y, ProjectileType type);
    ~Projectile() = default;
    
    float getVelocity() {
        return velocity;
    }
    
    ProjectileType getType() {
        return type;
    }
    
    Vector2f getPosition() {
        return position;
    }
    
    Sprite* getSprite() {
        return sprite;
    }
    
    void setVelocity(float v) {
        this->velocity = v;
    }
    
    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }
    
    void setProjectileType(ProjectileType type) {
        this->type = type;
    }
};

#endif /* Projectile_hpp */
