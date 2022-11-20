//#pragma once

#ifndef GameEntity_hpp
#define GameEntity_hpp

#include <stdio.h>
#include <map>
#include <SFML/Graphics.hpp>
#include "Weapon.hpp"
#include "Circle.hpp"
#include "Constants.h"
//#include "GameEntityMovement.hpp"
//class GameEntityMovement;
class GameEntityMovemennt;

enum class MoveDirection { DOWN, RIGHT, LEFT, UP };

using namespace std;
using namespace sf;

const int MALE_GENDER = 0;
const int FEMALE_GENDER = 1;

class GameEntity {
protected:
    long id;
    char name[10];
    int level;
    int gender; // 0 for male, 1 for female
    int currentHealthPoints;
    int maxHealthPoints;
    int currentManaPoints;
    int maxManaPoints;
    int attackPoints;
    int defencePoints;
    int currentDefencePoints;
    float speed;
    bool inBattle;
    bool dead = false;
    MoveDirection moveDirection;
    map<MoveDirection, int> moveDirectionsSpritesMap;
    Vector2f position;
    Texture texture;
    Sprite* sprite; // maybe VertexArray for each direction
    Circle* entityCircle = nullptr;
    Circle* attackRangeCircle = nullptr;
//    GameMap* currentGameMap;
    Weapon* weapon;
    
private:
//    GameEntityMovement* movement;
    
public:
    GameEntity();
    ~GameEntity() = default;
    // getters
    long getID();
    char* getName();
    int getLevel();
    int getGender();
    int getMaxHealthPoints();
    int getCurrentHealthPoints();
    int getMaxManaPoints();
    int getCurrentManaPoints();
    int getAttackPoints();
    int getDefencePoints();
    int getCurrentDefencePoints();
    float getSpeed();
    int getStep();
    bool isInBattle();
    bool isDead();
    MoveDirection getMoveDirection();
    map<MoveDirection, int> getMoveDirectionsSpritesMap();
    // counter for sprite change (0 to 3)
    int step = 0;
    Vector2f getPosition();
    Sprite* getSprite();
    FloatRect getRectangle(); // sprite.getGlobalBounds()
    
    void increaseLevel(int amount);
    void increaseMaxHealthPoints(int amount);
    void increaseMaxManaPoints(int amount);
    void increaseSpeed(float amount);
    void increaseAttackPoints(int amount);
    void increaseDefencePoints(int amount);
    void changeInBattleState();
    void setMoveDirection(MoveDirection direction);
    void incrementStep();
    void setX(float x);
    void setY(float y);
    void setPosition(float x, float y);
    void setWeapon(WeaponType type);
    
    void setIsInBattle(bool inBattle);

    // for situations where changing for worse equipment (adding logic for negative base values)
    void decreaseMaxHealthPoints(int amount);
    void decreaseMaxManaPoints(int amount);
    void decreaseCurrentHealthPoints(int amount);
    void decreaseCurrentManaPoints(int amount);
    void decreaseSpeed(float speed);
    void decreaseAttackPoints(int amount);
    void decreaseDefencePoints(int amount);
    void decreaseCurrentDefencePoints(int amount);
    
    bool isEntityInAttackRange(GameEntity &entity);
    bool intersects(GameEntity &entity);
    Circle* getCircle();
    Circle* getAttackRangeCircle();
    
    void attack(GameEntity &entity);
    bool move(MoveDirection direction);
    
    void update();
};

#endif /* GameEntity_hpp */
