#ifndef GameEntity_hpp
#define GameEntity_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "GameMap.hpp"
#include "Game.hpp"

enum class MoveDirection { UP, DOWN, RIGHT, LEFT };

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
    float speed;
    bool inBattle;
    MoveDirection moveDirection;
    Vector2f position;
    Sprite sprite; // maybe VertexArray for each direction
    GameMap* currentGameMap;
//    FloatRect rectangle; for intersecting with unreachable areas
    Game* game;
    
public:
    GameEntity(Game* game);
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
    float getSpeed();
    bool isInBattle();
    MoveDirection getMoveDirection();
    Vector2f getPosition();
    Sprite getSprite();
    GameMap* getCurrentGameMap();
    FloatRect getRectangle(); // sprite.getGlobalBounds()
    
    void increaseLevel(int amount);
    void increaseMaxHealthPoints(int amount);
    void increaseMaxManaPoints(int amount);
    void increaseSpeed(float amount);
    void increaseAttackPoints(int amount);
    void increaseDefencePoints(int amount);
    void changeInBattleState();
    void setMoveDirection(MoveDirection direction);
    void setPosition(float x, float y);
    void setCurrentGameMap(GameMap &map);
    // for situations where changing for worse equipment (adding logic for negative base values)
    void decreaseMaxHealthPoints(int amount);
    void decreaseMaxManaPoints(int amount);
    void decreaseSpeed(float speed);
    void decreaseAttackPoints(int amount);
    void decreaseDefencePoints(int amount);
    
    Game* getGame();
};

#endif /* GameEntity_hpp */
