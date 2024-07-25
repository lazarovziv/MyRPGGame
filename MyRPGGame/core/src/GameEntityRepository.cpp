#include "../include/GameEntityRepository.hpp"

GameEntityRepository::GameEntityRepository(std::unique_ptr<GameEntityMovement> movement,
                                   std::unique_ptr<GameEntityBattle> battle)
                                   : movement_handler(std::move(movement)), battle_handler(std::move(battle)) {}

void GameEntityRepository::set_game_map(std::shared_ptr<GameMap> gameMap) {
    // changing to the new map (move cause for crash?)
    map = std::move(gameMap); // transferring ownership of the gameMap pointer to the repository
    // updating movement handler
    movement_handler->set_current_map(map);
}

bool GameEntityRepository::jump(const physics::Vector &direction, const real dt) {
    return movement_handler->jump(direction, dt);
}
