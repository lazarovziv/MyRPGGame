#include "../include/PlayerRepository.hpp"

PlayerRepository::PlayerRepository(const std::shared_ptr<Player> &player, GameEntityMovement &movement,
                                   GameEntityBattle &battle, const std::shared_ptr<GameMap> &gameMap) :
                                   movementHandler(movement), battleHandler(battle) {
    this->player = player;
    setGameMap(gameMap);
    animationManager = new AnimationManager(this->player.get());
}

void PlayerRepository::setGameMap(std::shared_ptr<GameMap> gameMap) {
    // setting to-be-replaced map's player to null
    if (map != nullptr) map->removePlayer();
    // changing to the new map (move cause for crash?)
    map = std::move(gameMap); // transferring ownership of the gameMap pointer to PlayerRepository
    map->setPlayer(player);
    // updating movement handler
    movementHandler.setCurrentMap(map);
}

bool PlayerRepository::move(physics::Vector &direction, const bool run, const real dt) {
    player->setIsRunning(run);
    // movement handler expects a normalized vector
    direction.normalize();
    if (movementHandler.move(direction, dt) != Constants::MoveSuccessValues::SUCCESS) return false;
    animationManager->animate(run ? EntityMovementState::RUN : EntityMovementState::WALK, dt);
    return true;
}

bool PlayerRepository::jump(physics::Vector &direction, const real dt) {
    if (!player->isJumping()) {
        player->setIsJumping(true);
    }
    player->jump(direction, dt); // TODO: change to use movementHandler
    animationManager->animate(EntityMovementState::JUMP, dt);
    return true;
}

// TODO: add listeners invocation
bool PlayerRepository::attack(const EntityMovementState state, const real dt) {
    if (!player->canAttack()) return false;
    // succeeded in attacking any of the enemies
    bool singleSuccess = false;
    // TODO: instead of traversing all enemies, maybe keep a min heap which is ordered by the distance from the player
    for (int i = 0; i < map->getEnemies().size(); i++) {
        if (!map->getEnemies().at(i)->isDead()) {
             singleSuccess = player->isAttacking() || battleHandler.attack(*(map->getEnemies()[i]), dt);
        }
    }
    // resetting move interval because player isn't idle
    if (singleSuccess) {
        player->setIsAttacking(true);
        animationManager->animate(state, dt);
        player->resetMoveInterval();
        // player->resetBattleInterval();
    }

    return singleSuccess;
}

void PlayerRepository::update(const real dt) {
    player->update(dt);
    // player->getWeapon()->update(dt);
    player->notifyAll();
}