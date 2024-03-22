#include "../include/GameEntityBattle.hpp"

GameEntityBattle::GameEntityBattle(GameEntity *entity) {
    this->entity = entity;
    animationManager = std::make_unique<AnimationManager>(this->entity);
}

GameEntity *GameEntityBattle::getEntity() {
    return entity;
}

void GameEntityBattle::setEntity(GameEntity *gameEntity) {
    entity = gameEntity;
    animationManager->setEntity(entity);
}

bool GameEntityBattle::attack(GameEntity &enemy, const real dt) {
    // checking attack interval
    if (!entity->canAttack()) return false;
    // checking attack range
    if (/*isInAttackRange(enemy)*/entity->canAttack()) {
        real SCALE = 100;
        // setting entities' battle state
        entity->setIsInBattle(true);
        enemy.setIsInBattle(true);
        // lower defence points before health points
        if (enemy.getCurrentDefencePoints() > 0) {
            real defenceAttackPtsDiff = (entity->getAttackPoints() * dt * SCALE) - enemy.getCurrentDefencePoints();
            // if attack will break enemy's defence
            if (defenceAttackPtsDiff > 0) {
                // zeroing defence points
                enemy.decreaseCurrentDefencePoints(enemy.getCurrentDefencePoints());
                // decrease enemy's health points by difference
                enemy.decreaseCurrentHealthPoints(defenceAttackPtsDiff);
                // TODO: set enemy image as hurt (create force to bounce back for each weapon?)
                // decrease defence points by attack points
            } else enemy.decreaseCurrentDefencePoints(entity->getAttackPoints() * dt * SCALE);
            // broke defence points
        } else {
            enemy.decreaseCurrentHealthPoints(entity->getAttackPoints() * dt * SCALE);
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

    std::cout << "Health: " << enemy.getCurrentHealthPoints() << std::endl;
    std::cout << "Defence: " << enemy.getCurrentDefencePoints() << std::endl;
//    entity->resetBattleInterval(); // moved to repository
    return true;
}
