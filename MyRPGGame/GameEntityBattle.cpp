#include "GameEntityBattle.hpp"

GameEntityBattle::GameEntityBattle(GameEntity* entity) {
    this->entity = entity;
}

bool GameEntityBattle::attack(GameEntity &enemy) {
    if (isInAttackRange(enemy)) {
        // setting entities' battle state
        entity->setIsInBattle(true);
        enemy.setIsInBattle(true);
//        std::cout << "In attack range!" << endl;
        if (enemy.getCurrentDefencePoints() > 0) {
            int defenceAttackPtsDiff = entity->getAttackPoints() - enemy.getCurrentDefencePoints();
            // if attack will break enemy's defence
            if (defenceAttackPtsDiff > 0) {
                // zeroing defence points
                enemy.decreaseCurrentDefencePoints(enemy.getCurrentDefencePoints());
                // decrease enemy's health points by difference
                enemy.decreaseCurrentHealthPoints(defenceAttackPtsDiff);
                // TODO: set enemy image as hurt
                // decrease defence points by attack points
            } else enemy.decreaseCurrentDefencePoints(entity->getAttackPoints());
            // broke defence points
        } else {
            enemy.decreaseCurrentHealthPoints(entity->getAttackPoints());
            // changing battle state for entities
            if (enemy.isDead()) {
                enemy.setIsInBattle(false);
                entity->setIsInBattle(false);
            }
        }
    } else return false;
    return true;
}

bool GameEntityBattle::isInAttackRange(GameEntity &enemy) {
    return entity->getAttackRangeCircle()->intersects(*(enemy.getCircle()));
}
