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
//#include "GameEntityMovement.hpp"
//class GameEntityMovement;
class GameEntityMovement;

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
    real speed;
    int maxStaminaPoints;
    int currentStaminaPoints;

    // counter for sprite change (0 to 3)
    int step = 0;
    bool inBattle = false;
    bool dead = false;
    MoveDirection moveDirection;
    EntityMovementState movementState;
    std::map<MoveDirection, int> moveDirectionsSpritesMap;
    std::map<EntityMovementState, int> movementStateRowMap;

    sf::Vector2f position;
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
//    sf::Sprite *sprite; // maybe VertexArray for each direction
    // for handling changing in entity's movement (from walking to running, from idle to combat etc.)
    std::vector<sf::Texture> movementStateTextures;
    std::map<EntityMovementState, sf::Sprite*> movementStateSpritesMap;
    sf::IntRect spriteRect;
    std::unique_ptr<Circle> entityCircle = nullptr;
    std::unique_ptr<Circle> attackRangeCircle = nullptr;
    // circle positioned in potentially next steps for the player (instead of creating new ones in move method)
    std::unique_ptr<Circle> entityRightCircle, entityLeftCircle, entityTopCircle, entityBottomCircle;
//    GameMap* currentGameMap;
    std::unique_ptr<Weapon> weapon;

    // combat intervals
    constexpr static const real BATTLE_INTERVAL_DEFAULT = 9.0f;
    real battleInterval = 0.f;
    bool justMoved = false;

    // movement intervals
    constexpr static const real MOVE_INTERVAL_DEFAULT = 3.0f;
    real moveInterval = 0.f;
    bool idle = true;
    // for the animations. after traveled the speed distance, increment relevant animation count
    real distanceTraveledSinceIdle = 0;

    // idle interval
    real idleAnimationInterval = 0;

    std::stack<Point *> movesStack;
    
private:
//    GameEntityMovement* movement;
    
public:
    GameEntity();
    explicit GameEntity(Point *center);
    virtual ~GameEntity();
    // getters
    long getID() const;
    char* getName();
    int getLevel() const;
    int getGender() const;
    int getMaxHealthPoints() const;
    int getCurrentHealthPoints() const;
    int getMaxManaPoints() const;
    int getCurrentManaPoints() const;
    int getAttackPoints() const;
    int getDefencePoints() const;
    int getCurrentDefencePoints() const;
    int getMaxStaminaPoints() const;
    int getCurrentStaminaPoints() const;
    real getSpeed() const;
    int getStep() const;
    bool isInBattle() const;
    bool isDead() const;
    MoveDirection getMoveDirection() const;
    EntityMovementState getMovementState() const;
    std::map<MoveDirection, int> getMoveDirectionsSpritesMap() const;
    std::map<EntityMovementState, int> getMovementStateRowMap();
    sf::Vector2f getPosition() const;
    sf::Sprite* getSprite() const;
    sf::IntRect getRectangle() const; // sprite.getGlobalBounds()
    void setIntRectPosition(int left, int top, int width, int height);
    
    void increaseLevel(int amount);
    void increaseMaxHealthPoints(int amount);
    void increaseMaxManaPoints(int amount);
    void increaseSpeed(real amount);
    void setSpeed(real newSpeed);
    void increaseAttackPoints(int amount);
    void increaseDefencePoints(int amount);
    void changeInBattleState();
    void setMoveDirection(MoveDirection direction);
    void setMovementState(EntityMovementState state);
    void incrementStep();
    void setX(real x);
    void setY(real y);
    void setPosition(real x, real y);
    void setPosition(Point *point);
    void setPosition(sf::Vector2f directionVector, real dt);

    bool createMovementStateSprite(EntityMovementState state);
    bool addMovementStateSprite(EntityMovementState state, sf::Sprite *newSprite); // if sprite is null, we'll create one based on the state
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

    sf::Sprite* getMovementStateSprite(EntityMovementState state);
    void setSprite(sf::Sprite *newSprite);
    Weapon *getWeapon();

    bool canAttack() const;
    void resetBattleInterval();
    bool didJustMove() const;
    void setJustMoved(bool flag);
    bool isIdle() const;
    void resetDistanceTraveledSinceIdle();
    void incrementDistanceTraveledSinceIdle(real distance);
    bool canAnimateMovement();

    bool canAnimateIdle();
    void resetIdleAnimationInterval();
    void incrementIdleAnimationInterval(real dt);

    bool canGoIdle() const;
    void resetMoveInterval();
    void setIsIdle(bool flag);

    void setLastTimeMoved(std::clock_t time);

    void pushToMoveStack(Point *move);
    Point *popMove();
    bool areAvailableMoves();
    int numOfMovesAvailable();
    void clearMoveStack();
    
    void update(Point ***points, real dt);
};

#endif /* GameEntity_hpp */
