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
#include "Constants.h"
//#include "GameEntityMovement.hpp"
//class GameEntityMovement;
class GameEntityMovement;

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
    int speed;
    // counter for sprite change (0 to 3)
    int step = 0;
    bool inBattle = false;
    bool dead = false;
    MoveDirection moveDirection;
    map<MoveDirection, int> moveDirectionsSpritesMap;
    map<EntityMovementState, int> movementStateRowMap;

    Vector2f position;
    Texture texture;
    Sprite *sprite; // maybe VertexArray for each direction
    // for handling changing in entity's movement (from walking to running, from idle to combat etc.)
    vector<Texture> movementStateTextures;
    map<EntityMovementState, Sprite*> movementStateSpritesMap;
    IntRect spriteRect;
    Circle *entityCircle = nullptr;
    Circle *attackRangeCircle = nullptr;
//    GameMap* currentGameMap;
    Weapon *weapon;

    std::clock_t lastTimeBattled;
    float battleTimeout = 0.35;
    bool justMoved = false;

    // movement intervals
    std::clock_t lastTimeMoved;
    constexpr static const float MOVE_INTERVAL_DEFAULT = 0.075;
    float moveInterval = MOVE_INTERVAL_DEFAULT;
    bool idle = true;

    stack<Point *> movesStack;
    
private:
//    GameEntityMovement* movement;
    
public:
    GameEntity();
    explicit GameEntity(Point *center);
    virtual ~GameEntity();
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
    int getSpeed();
    int getStep();
    bool isInBattle();
    bool isDead();
    MoveDirection getMoveDirection();
    map<MoveDirection, int> getMoveDirectionsSpritesMap();
    map<EntityMovementState, int> getMovementStateRowMap();
    Vector2f getPosition();
    Sprite* getSprite();
    IntRect getRectangle(); // sprite.getGlobalBounds()
    void setIntRectPosition(int left, int top, int width, int height);
    
    void increaseLevel(int amount);
    void increaseMaxHealthPoints(int amount);
    void increaseMaxManaPoints(int amount);
    void increaseSpeed(int amount);
    void increaseAttackPoints(int amount);
    void increaseDefencePoints(int amount);
    void changeInBattleState();
    void setMoveDirection(MoveDirection direction);
    void incrementStep();
    void setX(float x);
    void setY(float y);
    void setPosition(float x, float y);
    void setPosition(Point *point);
    bool createMovementStateSprite(EntityMovementState state);
    bool addMovementStateSprite(EntityMovementState state, Sprite *newSprite); // if sprite is null, we'll create one based on the state
    void setWeapon(WeaponType type);
    void setIsInBattle(bool inBattle);

    // for situations where changing for worse equipment (adding logic for negative base values)
    void decreaseMaxHealthPoints(int amount);
    void decreaseMaxManaPoints(int amount);
    void decreaseCurrentHealthPoints(int amount);
    void decreaseCurrentManaPoints(int amount);
    void decreaseSpeed(int speed);
    void decreaseAttackPoints(int amount);
    void decreaseDefencePoints(int amount);
    void decreaseCurrentDefencePoints(int amount);
    
    bool isEntityInAttackRange(GameEntity &entity);
    bool intersects(GameEntity &entity);
    Circle* getCircle();
    Circle* getAttackRangeCircle();

    Sprite* getMovementStateSprite(EntityMovementState state);
    void setSprite(Sprite *newSprite);
    Weapon *getWeapon();
    
    void attack(GameEntity &entity);
    bool move(MoveDirection direction);

    bool canAttack();
    bool didJustMove();
    void setJustMoved(bool flag);
    bool isIdle();

    bool canGoIdle();
    void setIsIdle(bool flag);

    void setLastTimeMoved(std::clock_t time);

    void pushToMoveStack(Point *move);
    Point *popMove();
    bool areAvailableMoves();
    int numOfMovesAvailable();
    void clearMoveStack();
    
    virtual void update(Point ***points, float dt);
};

#endif /* GameEntity_hpp */
