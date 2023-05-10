#include "../include/PlayerRepository.hpp"

PlayerRepository::PlayerRepository(Player *player, GameMap *gameMap) {
    this->player = player;
    map = gameMap;
    movementHandler = new GameEntityMovement(player, true, map);
    battleHandler = new GameEntityBattle(player);
}

void PlayerRepository::setGameMap(GameMap *gameMap) {
    map = gameMap;
}

// TODO: add listeners invocation
bool PlayerRepository::move(MoveDirection direction) {
    return movementHandler->move(direction);
}

// TODO: add listeners invocation
bool PlayerRepository::attack(GameEntity &enemy) {
    return battleHandler->attack(enemy);
}