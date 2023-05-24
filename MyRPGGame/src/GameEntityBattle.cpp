#include "../include/GameEntityBattle.hpp"

GameEntityBattle::GameEntityBattle(GameEntity* entity) {
    this->entity = entity;
}

GameEntity *GameEntityBattle::getEntity() {
    return entity;
}

void GameEntityBattle::setEntity(GameEntity *gameEntity) {
    entity = gameEntity;
}

bool GameEntityBattle::attack(GameEntity &enemy) {
    // checking attack interval
    if (!entity->canAttack()) return false;
    // checking attack range
    if (isInAttackRange(enemy)) {
        // setting entities' battle state
        entity->setIsInBattle(true);
        enemy.setIsInBattle(true);
        // lower defence points before health points
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
                // TODO: increase player's XP
            }
        }
    } else {
        // TODO: fix changing battle states for entities regardless attack attempt
//        enemy.setIsInBattle(false);
//        entity->setIsInBattle(false);
        return false;
    }
    cout << "Health: " << enemy.getCurrentHealthPoints() << endl;
    cout << "Defence: " << enemy.getCurrentDefencePoints() << endl;
    return true;
}

bool GameEntityBattle::isInAttackRange(GameEntity &enemy) {
    return entity->getAttackRangeCircle()->intersects(enemy.getCircle());
}
