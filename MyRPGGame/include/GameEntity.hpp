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
#include "RigidBody.hpp"

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
    real currentHealthPoints;
    real maxHealthPoints;
    real currentManaPoints;
    real maxManaPoints;
    real attackPoints;
    real defencePoints;
    real currentDefencePoints;
    real speed;
    real maxStaminaPoints;
    real currentStaminaPoints;

    bool isPlayer = false;
    bool isLandscape = false;
    // TODO: add flag for indicating whether the entity is on the ground
    // flag for idle state and time delta incrementation
    bool positionUpdated = false;

    // counter for sprite change (0 to 3)
    int step = 0;
    bool inBattle = false;
    bool dead = false;
    bool moving = true;
    bool running = false;
    bool attacking = false;
    bool jumping = false;
    MoveDirection moveDirection;
    EntityMovementState movementState;
    std::map<MoveDirection, int> moveDirectionsSpritesMap;
    std::map<EntityMovementState, int> movementStateRowMap;
    std::map<EntityMovementState, int> movementStateColMap;
//    EntityMovementState lastCombatStateUsed;
    // field for all collisions and position of the entity
    std::unique_ptr<physics::RigidBody> rigidBody;

    sf::Vector2f position;
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
//    sf::Sprite *sprite; // maybe VertexArray for each direction
    // for handling changing in entity's movement (from walking to running, from idle to combat etc.)
    std::vector<sf::Texture> movementStateTextures;
    std::map<EntityMovementState, sf::Sprite*> movementStateSpritesMap;
    sf::IntRect spriteRect;
//    GameMap* currentGameMap;
    std::unique_ptr<Weapon> weapon;

    // combat intervals between each frame in the swing
    constexpr static const real BATTLE_INTERVAL_DEFAULT = (real) 1.0f;
    real battleInterval = 0.f;
    bool justMoved = false;

    // interval between swings
    constexpr static const real SWING_INTERVAL_DEFAULT = (real) 8.0f;

    // change direction interval (for enemies only!)
    constexpr static const real CHANGE_MOVE_DIRECTION_INTERVAL = 256.0f;
    real changeMoveDirectionInterval = 0;

    // movement intervals
    constexpr static const real MOVE_INTERVAL_DEFAULT = 3.0f;
    real moveInterval = 0.f;
    bool idle = true;
    // for the animations. after traveled the speed distance, increment relevant animation count
    real distanceTraveledSinceIdle = 0;
    // idle interval
    real idleAnimationInterval = 0;

    constexpr static const real JUMP_HEIGHT_INTERVAL_DEFAULT = 4.0f;
    // for between jumps
    constexpr static const real JUMP_INTERVAL_DEFAULT = 48.0f;
    real jumpHeightSinceOnGround = 0;

    std::stack<Point *> movesStack;
    
private:
//    GameEntityMovement* movement;
    
public:
    GameEntity();
    explicit GameEntity(physics::Vector initialPosition, physics::RigidBodyType rigidBodyType,
                        const std::vector<physics::Vector> &vertices = {}, real mass = 1);
    virtual ~GameEntity();
    // getters
    long getID() const;
    char* getName();
    int getLevel() const;
    int getGender() const;
    real getMaxHealthPoints() const;
    real getCurrentHealthPoints() const;
    real getMaxManaPoints() const;
    real getCurrentManaPoints() const;
    real getAttackPoints() const;
    real getDefencePoints() const;
    real getCurrentDefencePoints() const;
    real getMaxStaminaPoints() const;
    real getCurrentStaminaPoints() const;
    real getSpeed() const;
    int getStep() const;
    bool isInBattle() const;
    bool isDead() const;
    MoveDirection getMoveDirection() const;
    EntityMovementState getMovementState() const;
    std::map<MoveDirection, int> getMoveDirectionsSpritesMap() const;
    int getMovementStateColCount(EntityMovementState state) const;
    void incrementMovementStateColCount(EntityMovementState state);
    void resetMovementStateColCount(EntityMovementState state);
    std::map<EntityMovementState, int> getMovementStateRowMap();
    sf::Vector2f getRenderPosition() const;
    sf::Sprite* getSprite() const;
    sf::IntRect getRectangle() const; // sprite.getGlobalBounds()
    void setIntRectPosition(int left, int top, int width, int height);

    physics::RigidBody *getRigidBody() const;
    physics::Vector getPosition() const;
    
    void increaseLevel(int amount);
    void increaseMaxHealthPoints(int amount);
    void increaseMaxManaPoints(int amount);
    void increaseSpeed(const real amount);
    void setSpeed(real newSpeed);
    void increaseAttackPoints(int amount);
    void increaseDefencePoints(int amount);
    void changeInBattleState();
    void setMoveDirection(MoveDirection direction);
    void setMovementState(EntityMovementState state);
    void incrementStep();
    void setX(real x);
    void setY(real y);
    void setPosition(real x, real y, real z = 0);
    void setPosition(const physics::Vector &newPosition);
    void move(const physics::Vector &directionVector, real dt);
    void jump(physics::Vector &directionVector, real dt);
    void setMoveDirection(physics::Vector directionVector);

    bool createMovementStateSprite(EntityMovementState state);
    bool addMovementStateSprite(EntityMovementState state, sf::Sprite *newSprite); // if sprite is null, we'll create one based on the state
    void setWeapon(WeaponType type);
    void setIsInBattle(bool inBattle);

    // for situations where changing for worse equipment (adding logic for negative base values)
    void decreaseMaxHealthPoints(real amount);
    void decreaseMaxManaPoints(real amount);
    void decreaseCurrentHealthPoints(real amount);
    void decreaseCurrentManaPoints(real amount);
    void decreaseSpeed(real speed);
    void decreaseAttackPoints(real amount);
    void decreaseDefencePoints(real amount);
    void decreaseCurrentDefencePoints(real amount);

    sf::Sprite* getMovementStateSprite(EntityMovementState state);
    void setSprite(sf::Sprite *newSprite);
    Weapon *getWeapon();

    bool isRunning() const;
    void setIsRunning(bool flag);
    bool isMoving() const;
    void setIsMoving(bool flag);
    bool isAttacking() const;
    void setIsAttacking(bool flag);
    bool isJumping() const;
    void setIsJumping(bool flag);
    bool canAttack() const;
    void resetBattleInterval();
    void increaseBattleInterval(real dt);
    void resetBattleIntervalForSwing();
    bool didJustMove() const;
    void setJustMoved(bool flag);
    bool isIdle() const;
    void resetDistanceTraveledSinceIdle();
    void incrementDistanceTraveledSinceIdle(real distance);
    void incrementJumpHeightSinceOnGround(real distance);
    bool canAnimateMovement(bool check = false);

    bool canAnimateIdle(bool check = false);
    void resetIdleAnimationInterval();
    void resetJumpHeightSinceOnGroundInterval();
    void resetJumpInterval();
    void incrementIdleAnimationInterval(real dt);

    bool canAnimateJump(bool check = false);

    bool canGoIdle() const;
    bool canChangeDirection() const;
    void resetChangeDirectionInterval();
    void resetMoveInterval();
    void setIsIdle(bool flag);

    void printPosition() const;
    
    void update(real dt);
};

#endif /* GameEntity_hpp */
