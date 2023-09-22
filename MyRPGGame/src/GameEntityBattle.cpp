#include "../include/GameEntityBattle.hpp"

GameEntityBattle::GameEntityBattle(GameEntity* entity) {
    this->entity = entity;
    animationManager = new AnimationManager(this->entity);
}

GameEntity *GameEntityBattle::getEntity() {
    return entity;
}

void GameEntityBattle::setEntity(GameEntity *gameEntity) {
    entity = gameEntity;
    animationManager->setEntity(entity);
}

// TODO: add a parameter for attack type (slash, backslash, halfslash, etc.)
bool GameEntityBattle::animate(real dt) {
//    Weapon *weapon = entity->getWeapon();
//    MoveDirection direction = weapon->getTransitionDirection();
    MoveDirection direction = entity->getMoveDirection();
    int directionRow = entity->getMoveDirectionsSpritesMap()[direction];
    // AttackSuccessValue attackSuccessValue;
    std::cout << "Attacking" << std::endl;
    // set attack success value
    animationManager->incrementCombatSlashOneHandedCount();
    int movementStateCount = round(animationManager->getMovementStateCount(EntityMovementState::COMBAT_SLASH_ONE_HANDED) * dt * 1.74f);

    entity->setIntRectPosition(movementStateCount * Constants::TILE_SIZE,
                               (entity->getMovementStateRowMap()[EntityMovementState::COMBAT_SLASH_ONE_HANDED] + directionRow-1) * Constants::TILE_SIZE,
                               Constants::TILE_SIZE, Constants::TILE_SIZE);
    return true;
}

bool GameEntityBattle::attack(GameEntity &enemy, real dt) {
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

    animate(dt); // animation
    std::cout << "Health: " << enemy.getCurrentHealthPoints() << std::endl;
    std::cout << "Defence: " << enemy.getCurrentDefencePoints() << std::endl;
//    entity->resetBattleInterval(); // moved to repository
    return true;
}

bool GameEntityBattle::isInAttackRange(GameEntity &enemy) {
    return entity->getAttackRangeCircle()->intersects(enemy.getCircle());
}
