#include "../include/AnimationManager.hpp"

AnimationManager::AnimationManager(GameEntity *entity) {
    this->entity = entity;
//    weapon = this->entity->getWeapon();
    movement_states.push_back("idle");
    movement_states.push_back("jump");
    movement_states.push_back("run");
    movement_states.push_back("sitting");
    movement_states.push_back("walk");
    movement_states.push_back("climb");
    movement_states.push_back("combat_1h_backslash");
    movement_states.push_back("combat_1h_halfslash");
    movement_states.push_back("combat_1h_idle");
    movement_states.push_back("combat_1h_slash");
    // filling the increment functions pointers map
    EntityMovementState entity_movement_state = EntityMovementState::CLIMB;
    while (entity_movement_state != EntityMovementState::IDLE) {
        increment_movement_state_count_function_map[entity_movement_state] = [&]() { increment_count(entity_movement_state); };
        movement_state_counter_map[entity_movement_state] = 0;

        entity_movement_state = static_cast<EntityMovementState>(static_cast<int>(entity_movement_state) + 1);
    }
    // entity_movement_state is now IDLE so we can use it here as the while loop stopped
    increment_movement_state_count_function_map[entity_movement_state] = [&]() { increment_count(entity_movement_state); };
    movement_state_counter_map[entity_movement_state] = 0;
}

void AnimationManager::add_body_path(std::string path) {
    body_paths.push_back(path);
    animations_paths[AnimationPathType::BODY] = path;
    animations_paths[AnimationPathType::HEAD] = Constants::GRAPHICS_BASE_PATH + "Characters/Head/masculine/idle.png";
}

void AnimationManager::add_clothing_path(std::string path) {
    clothing_paths.push_back(path);
}

void AnimationManager::add_head_accessories_path(std::string path) {
    head_accessories_paths.push_back(path);
}

void AnimationManager::add_hair_path(std::string path) {
    hair_paths.push_back(path);
}

void AnimationManager::set_entity(GameEntity *entity) {
    this->entity = entity;
}

int AnimationManager::get_movement_state_count(EntityMovementState state) {
    return movement_state_counter_map[state];
}

bool AnimationManager::is_movement_state(const EntityMovementState state) const {
    return state == EntityMovementState::WALK || state == EntityMovementState::RUN;
}

bool AnimationManager::is_combat_state(const EntityMovementState state) const {
    return state == EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED ||
    state == EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED ||
    state == EntityMovementState::COMBAT_IDLE_ONE_HANDED ||
    state == EntityMovementState::COMBAT_SLASH_ONE_HANDED;
}

// TODO: create animation function for combat and for that another entity variable to cover the combats' counters
// TODO: make function animate only, implement all intervals elsewhere (repository for example)
// TODO: move all unnecessary logic from animate method to another function which is not in the animation manager, i.e, make animation manager manage animation ONLY
void AnimationManager::animate(const EntityMovementState state, const real dt) {
    entity->set_movement_state(state);
    // using the row generically
    int direction_row = entity->get_move_directions_sprites_map()[entity->get_move_direction()] - 1;
    int entity_movement_state_col_count = entity->get_movement_state_col_count(state);
    bool is_last_column = entity_movement_state_col_count == Constants::MOVEMENT_STATE_NUM_COLS.at(state) - 1;
    bool animate = false;
    real origin_scale = 0.5;
    int tile_scale = 1;
    // function pointers for after the animation has finished (one sprite of it)
    void (GameEntity::*finish_function)() = nullptr;
    void (GameEntity::*non_finish_function)() = nullptr;

    if (state == EntityMovementState::IDLE) {
        animate = entity->can_animate_idle();
        direction_row += entity->get_movement_state_row_map()[state];
    } else if (is_movement_state(state)) {
        animate = entity->is_moving() && entity->can_animate_movement();
        direction_row += entity->get_movement_state_row_map()[state];
        finish_function = &GameEntity::reset_moving;
    } else if (is_combat_state(state)) {
        animate = entity->is_attacking() && entity->can_attack();
        origin_scale = 1;
        tile_scale = 2;
        direction_row *= tile_scale;
        direction_row += Constants::COMBAT_SLASH_ONE_HANDED_ROW;
        finish_function = &GameEntity::reset_attacking;
        non_finish_function = &GameEntity::reset_battle_interval;
    } else if (state == EntityMovementState::JUMP) {
        animate = entity->is_jumping() && entity->can_animate_jump();
        direction_row += entity->get_movement_state_row_map()[state];
        finish_function = &GameEntity::reset_jumping;
        non_finish_function = &GameEntity::reset_jump_height_since_on_ground_interval;
    }

    if (animate) {
        entity->increment_movement_state_col_count(state);
        entity->set_int_rect_position(entity_movement_state_col_count * Constants::TILE_SIZE * tile_scale,
                                               direction_row * Constants::TILE_SIZE,
                                               Constants::TILE_SIZE * tile_scale,
                                               Constants::TILE_SIZE * tile_scale);
        entity->get_sprite()->setOrigin(Constants::TILE_SIZE * origin_scale, Constants::TILE_SIZE * origin_scale);
        // reached end of the entire animation
        if (is_last_column && finish_function != nullptr) {
            (*entity.*finish_function)();
        } else if (non_finish_function != nullptr) (*entity.*non_finish_function)();
    }
}

bool AnimationManager::generate_body() {
    for (std::string movement_state : movement_states) {
        std::cout << movement_state << std::endl;
        // using the generate_image.py script
        create_image(Constants::GRAPHICS_BASE_PATH + "Characters/Body/masculine/" + movement_state + ".png",
                    Constants::GRAPHICS_BASE_PATH + "Characters/Head/masculine/" + movement_state + ".png",
                    movement_state);
    }
    return true;
}

void AnimationManager::increment_count(const EntityMovementState state) {
    if (movement_state_counter_map[state] < Constants::MOVEMENT_STATE_NUM_COLS.at(state)-1) {
        movement_state_counter_map[state] = movement_state_counter_map[state] + 1;
        return;
    }
    movement_state_counter_map[state] = 0;
}

//// create for idle, walk, run, climb, combat, jump, sit, expressions
//bool AnimationManager::generateBodyTest() {
//    cv::Mat bodyImage, headImage, finalImage;
//    string finalPath;
//    for (string movementState : movementStates) {
//        cout << movementState << endl;
//        bodyImage = cv::imread("../graphics/Characters/Body/masculine/" + movementState + ".png");
//        headImage = cv::imread("../graphics/Characters/Head/masculine/" + movementState + ".png");
//
//        // putting headImage on top of bodyImage into finalImage
//        addWeighted(bodyImage, 1.0, headImage, 1.0, 0.0, finalImage);
//        // combat has different output
//        if (movementState.substr(0, 6) == "combat") {
//            finalPath = "../graphics/player/body/combat/body_" + movementState + ".png";
//        } else finalPath = "../graphics/player/body/" + movementState + "/body_" + movementState + ".png";
//        // writing was not successful
//        if (!imwrite(finalPath, finalImage)) {
//            return false;
//        }
//    }
//    // all went well
//    return true;
//}
