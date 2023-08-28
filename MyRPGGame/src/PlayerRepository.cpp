#include "../include/PlayerRepository.hpp"

PlayerRepository::PlayerRepository(Player *player, GameEntityMovement *movement,
                                   GameEntityBattle *battle, GameMap *gameMap) {
    this->player = player;
    movementHandler = movement;
    battleHandler = battle;
    setGameMap(gameMap);
//    movementHandler = new GameEntityMovement(player, true, map);
//    battleHandler = new GameEntityBattle(player);
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
Constants::MoveSuccessValues PlayerRepository::move(MoveDirection direction, EntityMovementState movementState) {
    if (movementState == EntityMovementState::IDLE && player->canGoIdle()) {
        player->setIsIdle(true);
    } else player->setIsIdle(false);
    return movementHandler->move(direction, movementState);
}

// TODO: add listeners invocation
bool PlayerRepository::attack() {
    // succeeded in attacking any of the enemies
    bool singleSuccess = false;
    // TODO: instead of traversing all enemies, maybe keep a min heap which is ordered by the distance from the player
    for (int i = 0; i < map->getEnemies().size(); i++) {
        if (!map->getEnemies().at(i)->isDead()) {
             singleSuccess = battleHandler->attack(*(map->getEnemies()[i]));
        }
    }
    return singleSuccess;
}

void PlayerRepository::update(Point ***points, Constants::MoveSuccessValues moveSuccessValue, float dt) {
    // setting justMoved attribute in respect to move attempt (or none)
    player->setJustMoved(moveSuccessValue != Constants::MoveSuccessValues::NOT_MOVED &&
    moveSuccessValue != Constants::MoveSuccessValues::FAILURE);
    player->update(points, dt);
}