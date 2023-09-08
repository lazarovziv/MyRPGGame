//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include "GameMap.hpp"
#include "AnimationManager.hpp"

class GameEntityMovement {
private:
    GameEntity *entity;
    GameMap *currentMap;
    Point ***gameMapsPoints;
    AnimationManager *animationManager;
    bool isPlayer;
    int screenWidth, screenHeight;
    int tileSize;

    // TODO: add delta time multiplication with *entitySpeed* for every movement which is not screen borders
    Constants::MoveSuccessValues moveUp(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);
    Constants::MoveSuccessValues moveDown(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);
    Constants::MoveSuccessValues moveRight(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);
    Constants::MoveSuccessValues moveLeft(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);
    Constants::MoveSuccessValues moveLeftUp(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);
    Constants::MoveSuccessValues moveLeftDown(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);
    Constants::MoveSuccessValues moveRightUp(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);
    Constants::MoveSuccessValues moveRightDown(GameMap* map, float entityX, float entityY, float entitySpeed, float dt);

    bool reachedLeftEndOfScreen(float entityX, float entitySpeed, float dt) const;
    bool reachedRightEndOfScreen(float entityX, float entitySpeed, float dt) const;
    bool reachedTopEndOfScreen(float entityY, float entitySpeed, float dt) const;
    bool reachedBottomEndOfScreen(float entityY, float entitySpeed, float dt) const;

    void normalizeVector(Vector2f *vector);

public:
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map);
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map, Point ***points);
    ~GameEntityMovement() = default;
    Constants::MoveSuccessValues move(MoveDirection direction, EntityMovementState movementState, float dt);
    // TODO: add run method (use acceleration, speed etc.)

    bool moveRandomly(int randomDirection, float dt);
    bool moveTowardsEntity(GameEntity *gameEntity, Graph<Point *> *graph);
    bool moveTowardsPoint(Point *point, Graph<Point *> *graph);
    bool moveBasedOnPoint(Point *point, float dt);

    void calculatePathTo(GameEntity *target, float dt);

    void setEntity(GameEntity &entity);
    GameEntity* getEntity();
    void setCurrentMap(GameMap *map);
};

#endif /* GameEntityMovement_hpp */
