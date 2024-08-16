#include "../include/GameEntityRepository.hpp"

GameEntityRepository::GameEntityRepository(std::unique_ptr<GameEntityMovement> movement,
                                   std::unique_ptr<GameEntityBattle> battle)
                                   : movementHandler(std::move(movement)), battleHandler(std::move(battle)) {}

void GameEntityRepository::setGameMap(std::shared_ptr<GameMap> gameMap) {
    // changing to the new map (move cause for crash?)
    map = std::move(gameMap); // transferring ownership of the gameMap pointer to the repository
    // updating movement handler
    movementHandler->setCurrentMap(map);
}

bool GameEntityRepository::jump(const physics::Vector &direction, const real dt) {
    return movementHandler->jump(direction, dt);
}
