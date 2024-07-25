#include "../include/PlayerRepository.hpp"

PlayerRepository::PlayerRepository(const std::shared_ptr<Player> &player, std::unique_ptr<GameEntityMovement> movement,
                                   std::unique_ptr<GameEntityBattle> battle, const std::shared_ptr<GameMap> &gameMap)
                                   : GameEntityRepository(std::move(movement), std::move(battle)) {
    this->player = player;
    animation_manager = new AnimationManager(this->player.get());
    set_game_map(gameMap);
}

void PlayerRepository::set_game_map(std::shared_ptr<GameMap> gameMap) {
    // setting to-be-replaced map's player to null
    if (map != nullptr) map->remove_player();
    GameEntityRepository::set_game_map(std::move(gameMap));
    map->set_player(player);
}

bool PlayerRepository::move(physics::Vector &direction, const bool run, const real dt) {
    // movement handler expects a normalized vector
    direction.normalize();
    if (movement_handler->move(direction, dt) != Constants::MoveSuccessValues::SUCCESS) return false;
    player->set_is_running(run);
    animation_manager->animate(run ? EntityMovementState::RUN : EntityMovementState::WALK, dt);
    return true;
}

// TODO: add listeners invocation
bool PlayerRepository::attack(const EntityMovementState state, const real dt) {
    if (!player->can_attack()) return false;
    // succeeded in attacking any of the enemies
    bool singleSuccess = false;
    // TODO: instead of traversing all enemies, maybe keep a min heap which is ordered by the distance from the player
    for (int i = 0; i < map->get_enemies().size(); i++) {
        if (!map->get_enemies().at(i)->is_dead()) {
             singleSuccess = player->is_attacking() || battle_handler->attack(*(map->get_enemies()[i]), dt);
        }
    }
    // resetting move interval because player isn't idle
    if (singleSuccess) {
        player->set_is_attacking(true);
        animation_manager->animate(state, dt);
        player->reset_move_interval();
        // player->resetBattleInterval();
    }

    return singleSuccess;
}

void PlayerRepository::update(const real dt) {
    player->update(dt);
    // player->getWeapon()->update(dt);
    player->notify_all();
}