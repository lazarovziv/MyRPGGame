#include "../include/PlayerRepository.hpp"

PlayerRepository::PlayerRepository(Player *player, GameEntityMovement *movement,
                                   GameEntityBattle *battle, GameMap *gameMap) {
    this->player = player;
    movementHandler = movement;
    battleHandler = battle;
    setGameMap(gameMap);
    animationManager = new AnimationManager(player);
}

void PlayerRepository::setGameMap(GameMap *gameMap) {
    // setting to-be-replaced map's player to null
    if (map != nullptr) map->setPlayer(nullptr);
    // changing to the new map
    map = gameMap;
    map->setPlayer(player);
    // updating movement handler
    movementHandler->setCurrentMap(map);
}

void PlayerRepository::setLastTimeMoved(std::clock_t time) {
    player->setLastTimeMoved(time);
}

// TODO: add listeners invocation
Constants::MoveSuccessValues PlayerRepository::move(MoveDirection direction, EntityMovementState movementState, float dt, bool diagonal) {
    Constants::MoveSuccessValues moved = movementHandler->move(direction, movementState, dt, diagonal);
    return moved;
    // TODO: add animate call here and remove animation handling from move method in GameEntityMovement
}

// TODO: add listeners invocation
bool PlayerRepository::attack(float dt) {
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

void PlayerRepository::update(Point ***points, Constants::MoveSuccessValues moveSuccessValue, float dt) {
    // setting justMoved attribute in respect to move attempt (or none)
    player->setJustMoved(moveSuccessValue != Constants::MoveSuccessValues::NOT_MOVED &&
    moveSuccessValue != Constants::MoveSuccessValues::FAILURE);
    player->update(points, dt);
}