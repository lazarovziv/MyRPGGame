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

// TODO: add listeners invocation
Constants::MoveSuccessValues PlayerRepository::move(MoveDirection direction) {
    return movementHandler->move(direction);
}

// TODO: add listeners invocation
bool PlayerRepository::attack() {
    // succeeded in attacking any of the enemies
    bool singleSuccess = false;
    for (int i = 0; i < map->getEnemies().size(); i++) {
        if (!map->getEnemies().at(i)->isDead()) {
            singleSuccess = battleHandler->attack(*(map->getEnemies()[i]));
        }
    }
    return singleSuccess;
}

void PlayerRepository::update(Point ***points, Constants::MoveSuccessValues moveSuccessValue) {
    // setting justMoved attribute in respect to move attempt (or none)
    if (moveSuccessValue == Constants::MoveSuccessValues::NOT_MOVED ||
        moveSuccessValue == Constants::MoveSuccessValues::FAILURE) player->setJustMoved(false);
    else player->setJustMoved(true);
    player->update(points);
}