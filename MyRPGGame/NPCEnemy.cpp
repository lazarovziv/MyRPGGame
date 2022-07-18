#include "NPCEnemy.hpp"

NPCEnemy::NPCEnemy() {
    
}

NPCEnemy::NPCEnemy(int type, float x, float y) {
    this->type = type;
    level = 1;
    maxHealthPoints = 20;
    currentHealthPoints = maxHealthPoints;
    maxManaPoints = 10;
    currentManaPoints = maxManaPoints;
    attackPoints = 2;
    defencePoints = 1;
    currentDefencePoints = defencePoints;
    speed = 1.f;
    expPointsWorth = 10;
    inBattle = false;
    dead = false;
    moveDirection = MoveDirection::UP;
    spawn(x, y);
    Texture texture;
    texture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/player.png");
    sprite.setTexture(texture);
    sprite.setOrigin(32, 32);
    sprite.setPosition(x, y);
    weapon = new Weapon(WeaponType::BARE_HANDED);
    entityCircle = new Circle(position.x, position.y, 32);
    attackRangeCircle = new Circle(position.x, position.y, (entityCircle->getRadius() * 5/3) + weapon->getHitRadius());
    /*
     switch (type) {
         case EnemyType::WORM:
             level = 1;
             maxHealthPoints = 20;
             currentHealthPoints = maxHealthPoints;
             maxManaPoints = 10;
             currentManaPoints = maxManaPoints;
             attackPoints = 2;
             defencePoints = 1;
             speed = 1.f;
     
             expPointsWorth = 10;
     
             inBattle = false;
             moveDirection = MoveDirection::UP;
             Texture texture;
             texture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/player.png");
             sprite.setTexture(texture);
             sprite.setOrigin(Game::TILE_SIZE/2, Game::TILE_SIZE/2);
             sprite.setPosition(x, y);
             break;
         case EnemyType::SNAKE:
             level = 2;
             maxHealthPoints = 30;
             currentHealthPoints = maxHealthPoints;
             maxManaPoints = 10;
             currentManaPoints = maxManaPoints;
             attackPoints = 5;
             defencePoints = 2;
             speed = 2.f;
             inBattle = false;
             moveDirection = MoveDirection::UP;
             break;
         case EnemyType::BIRD:
             level = 3;
             maxHealthPoints = 35;
             currentHealthPoints = maxHealthPoints;
             maxManaPoints = 10;
             currentManaPoints = maxManaPoints;
             attackPoints = 7;
             defencePoints = 4;
             speed = 4.f;
             inBattle = false;
             moveDirection = MoveDirection::UP;
             break;
         case EnemyType::ETC:
             break;
     }
      TODO: choose random floats in defined location radius for each enemy in map
     spawn(x, y);
     */
}

int NPCEnemy::getBattleTimeout() {
    return battleTimeout;
}

float NPCEnemy::getWanderAreaRadius() {
    return wanderAreaRadius;
}

float NPCEnemy::getBattleAreaRadius() {
    return battleAreaRadius;
}

int NPCEnemy::getExpPointsWorth() {
    return expPointsWorth;
}

int NPCEnemy::getType() {
    return type;
}

void NPCEnemy::spawn(float x, float y) {
    this->setPosition(x, y);
}
