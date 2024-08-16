#include "GameEntityBattle.hpp"

GameEntityBattle::GameEntityBattle(GameEntity *entity) {
    this->entity = entity;
    animationManager = std::make_unique<AnimationManager>(this->entity);
}

GameEntity *GameEntityBattle::get_entity() {
    return entity;
}

void GameEntityBattle::set_entity(GameEntity *gameEntity) {
    entity = gameEntity;
    animationManager->set_entity(entity);
}

bool GameEntityBattle::attack(GameEntity &enemy, const real dt) {
    // checking attack interval
    if (!entity->can_attack()) return false;
    // checking attack range
    if (/*isInAttackRange(enemy)*/entity->can_attack()) {
        real SCALE = 100;
        // setting entities' battle state
        entity->set_is_in_battle(true);
        enemy.set_is_in_battle(true);
        // lower defence points before health points
        if (enemy.get_current_defence_points() > 0) {
            real defenceAttackPtsDiff = (entity->get_attack_points() * dt * SCALE) - enemy.get_current_defence_points();
            // if attack will break enemy's defence
            if (defenceAttackPtsDiff > 0) {
                // zeroing defence points
                enemy.decrease_current_defence_points(enemy.get_current_defence_points());
                // decrease enemy's health points by difference
                enemy.decrease_current_health_points(defenceAttackPtsDiff);
                // TODO: set enemy image as hurt (create force to bounce back for each weapon?)
                // decrease defence points by attack points
            } else enemy.decrease_current_defence_points(entity->get_attack_points() * dt * SCALE);
            // broke defence points
        } else {
            enemy.decrease_current_health_points(entity->get_attack_points() * dt * SCALE);
            // changing battle state for entities
            if (enemy.is_dead()) {
                enemy.set_is_in_battle(false);
                entity->set_is_in_battle(false);
                // TODO: increase player's XP
            }
        }
    } else {
        // TODO: fix changing battle states for entities regardless attack attempt
//        enemy.setIsInBattle(false);
//        entity->setIsInBattle(false);
        return false;
    }

    std::cout << "Health: " << enemy.get_current_health_points() << std::endl;
    std::cout << "Defence: " << enemy.get_current_defence_points() << std::endl;
//    entity->resetBattleInterval(); // moved to repository
    return true;
}
