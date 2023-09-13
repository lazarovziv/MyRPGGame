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
    float speed;
    int maxStaminaPoints;
    int currentStaminaPoints;

    // counter for sprite change (0 to 3)
    int step = 0;
    bool inBattle = false;
    bool dead = false;
    MoveDirection moveDirection;
    EntityMovementState movementState;
    map<MoveDirection, int> moveDirectionsSpritesMap;
    map<EntityMovementState, int> movementStateRowMap;

    Vector2f position;
    Texture texture;
    Sprite *sprite; // maybe VertexArray for each direction
    // for handling changing in entity's movement (from walking to running, from idle to combat etc.)
    vector<Texture> movementStateTextures;
    map<EntityMovementState, Sprite*> movementStateSpritesMap;
    IntRect spriteRect;
    unique_ptr<Circle> entityCircle = nullptr;
    unique_ptr<Circle> attackRangeCircle = nullptr;
    // circle positioned in potentially next steps for the player (instead of creating new ones in move method)
    unique_ptr<Circle> entityRightCircle, entityLeftCircle, entityTopCircle, entityBottomCircle;
//    GameMap* currentGameMap;
    Weapon *weapon;

    // combat intervals
    constexpr static const float BATTLE_INTERVAL_DEFAULT = 9.0f;
    float battleInterval = 0.f;
    bool justMoved = false;

    // movement intervals
    constexpr static const float MOVE_INTERVAL_DEFAULT = 3.0f;
    float moveInterval = 0.f;
    bool idle = true;
    // for the animations. after traveled the speed distance, increment relevant animation count
    float distanceTraveledSinceIdle = 0;

    // idle interval
    float idleAnimationInterval = 0;

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
    int getMaxStaminaPoints();
    int getCurrentStaminaPoints();
    float getSpeed();
    int getStep();
    bool isInBattle();
    bool isDead();
    MoveDirection getMoveDirection();
    EntityMovementState getMovementState();
    map<MoveDirection, int> getMoveDirectionsSpritesMap();
    map<EntityMovementState, int> getMovementStateRowMap();
    Vector2f getPosition();
    Sprite* getSprite();
    IntRect getRectangle(); // sprite.getGlobalBounds()
    void setIntRectPosition(int left, int top, int width, int height);
    
    void increaseLevel(int amount);
    void increaseMaxHealthPoints(int amount);
    void increaseMaxManaPoints(int amount);
    void increaseSpeed(float amount);
    void setSpeed(float newSpeed);
    void increaseAttackPoints(int amount);
    void increaseDefencePoints(int amount);
    void changeInBattleState();
    void setMoveDirection(MoveDirection direction);
    void setMovementState(EntityMovementState state);
    void incrementStep();
    void setX(float x);
    void setY(float y);
    void setPosition(float x, float y);
    void setPosition(Point *point);
    void setPosition(Vector2f directionVector, float dt);

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
    Circle* getRightCircle();
    Circle* getLeftCircle();
    Circle* getTopCircle();
    Circle* getBottomCircle();
    Circle* getAttackRangeCircle();

    Sprite* getMovementStateSprite(EntityMovementState state);
    void setSprite(Sprite *newSprite);
    Weapon *getWeapon();

    bool move(MoveDirection direction);

    bool canAttack() const;
    void resetBattleInterval();
    bool didJustMove();
    void setJustMoved(bool flag);
    bool isIdle();
    void resetDistanceTraveledSinceIdle();
    void incrementDistanceTraveledSinceIdle(float distance);
    bool canAnimateMovement();

    bool canAnimateIdle();
    void resetIdleAnimationInterval();
    void incrementIdleAnimationInterval(float dt);

    bool canGoIdle() const;
    void resetMoveInterval();
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
