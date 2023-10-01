#include "../include/PlayerRepository.hpp"

PlayerRepository::PlayerRepository(const std::shared_ptr<Player> player, GameEntityMovement *movement,
                                   GameEntityBattle *battle, const std::shared_ptr<GameMap> gameMap) {
    this->player = player;
    movementHandler = movement;
    battleHandler = battle;
    setGameMap(gameMap);
    forceGenerator = std::make_unique<physics::RigidBodyGravity>(physics::Vector{0, (real) -9.81, 0});
    animationManager = new AnimationManager(player.get());
}

void PlayerRepository::setGameMap(std::shared_ptr<GameMap> gameMap) {
    // setting to-be-replaced map's player to null
    if (map != nullptr) map->removePlayer();
    // changing to the new map
    map = gameMap;
    map->setPlayer(player);
    // updating movement handler
    movementHandler->setCurrentMap(map);
}

bool PlayerRepository::move(physics::Vector direction, bool run, real dt) {
    player->setIsRunning(run);
    // vector is normalized in the movement handler
    if (movementHandler->move(direction, dt) != Constants::MoveSuccessValues::SUCCESS) return false;
    animationManager->animate(run ? EntityMovementState::RUN : EntityMovementState::WALK, dt);
    return true;
}

// TODO: add listeners invocation
bool PlayerRepository::attack(real dt) {
    if (!player->canAttack()) return false;
    // succeeded in attacking any of the enemies
    bool singleSuccess = false;
    // TODO: instead of traversing all enemies, maybe keep a min heap which is ordered by the distance from the player
    for (int i = 0; i < map->getEnemies().size(); i++) {
        if (!map->getEnemies().at(i)->isDead()) {
             singleSuccess = battleHandler->attack(*(map->getEnemies()[i]), dt);
        }
    }
    // resetting move interval because player isn't idle
    if (singleSuccess) {
//        animationManager->animate(EntityMovementState::COMBAT_SLASH_ONE_HANDED, dt);
        player->resetMoveInterval();
        player->resetBattleInterval();
    }
    return singleSuccess;
}

void PlayerRepository::update(Constants::MoveSuccessValues moveSuccessValue, real dt) {
    // setting justMoved attribute in respect to move attempt (or none)
    player->setJustMoved(moveSuccessValue != Constants::MoveSuccessValues::NOT_MOVED &&
    moveSuccessValue != Constants::MoveSuccessValues::FAILURE);
    // forceGenerator->update(player->getRigidBody(), dt);
    player->update(dt);
    // player->getWeapon()->update(dt);
    player->notifyAll();
}