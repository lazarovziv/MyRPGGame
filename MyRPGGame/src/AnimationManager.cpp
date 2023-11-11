#include "../include/AnimationManager.hpp"

AnimationManager::AnimationManager(GameEntity *entity) {
    this->entity = entity;
//    weapon = this->entity->getWeapon();
    movementStates.push_back("idle");
    movementStates.push_back("jump");
    movementStates.push_back("run");
    movementStates.push_back("sitting");
    movementStates.push_back("walk");
    movementStates.push_back("climb");
    movementStates.push_back("combat_1h_backslash");
    movementStates.push_back("combat_1h_halfslash");
    movementStates.push_back("combat_1h_idle");
    movementStates.push_back("combat_1h_slash");
    // filling the increment functions pointers map
    incrementMovementStateCountFunctionMap[EntityMovementState::IDLE] = [this]() { incrementIdleCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::JUMP] = [this]() { incrementJumpCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::RUN] = [this]() { incrementRunCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::SITTING] = [this]() { incrementSittingCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::WALK] = [this]() { incrementWalkCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::CLIMB] = [this]() { incrementClimbCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] =
            [this]() { incrementCombatIdleOneHandedCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] =
            [this]() { incrementCombatSlashOneHandedCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] =
            [this]() { incrementCombatBackslashOneHandedCount(); };
    incrementMovementStateCountFunctionMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] =
            [this]() { incrementCombatHalfslashOneHandedCount(); };

    movementStateCounterMap[EntityMovementState::IDLE] = 0;
    movementStateCounterMap[EntityMovementState::JUMP] = 0;
    movementStateCounterMap[EntityMovementState::RUN] = 0;
    movementStateCounterMap[EntityMovementState::SITTING] = 0;
    movementStateCounterMap[EntityMovementState::WALK] = 0;
    movementStateCounterMap[EntityMovementState::CLIMB] = 0;
    movementStateCounterMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = 0;
    movementStateCounterMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = 0;
    movementStateCounterMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = 0;
    movementStateCounterMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = 0;
}

void AnimationManager::addBodyPath(std::string path) {
    bodyPaths.push_back(path);
    animationsPaths[AnimationPathType::BODY] = path;
    animationsPaths[AnimationPathType::HEAD] = "../graphics/Characters/Head/masculine/idle.png";
}

void AnimationManager::addClothingPath(std::string path) {
    clothingPaths.push_back(path);
}

void AnimationManager::addHeadAccessoriesPath(std::string path) {
    headAccessoriesPaths.push_back(path);
}

void AnimationManager::addHairPath(std::string path) {
    hairPaths.push_back(path);
}

void AnimationManager::setEntity(GameEntity *entity) {
    this->entity = entity;
}

int AnimationManager::getMovementStateCount(EntityMovementState state) {
    return movementStateCounterMap[state];
}

/*
enum class EntityMovementState {
    CLIMB, COMBAT_BACKSLASH_ONE_HANDED, COMBAT_HALFSLASH_ONE_HANDED,
    COMBAT_IDLE_ONE_HANDED, COMBAT_SLASH_ONE_HANDED, WALK, JUMP, SITTING, RUN, IDLE };*/

bool AnimationManager::isMovementState(EntityMovementState state) const {
    return state == EntityMovementState::WALK || state == EntityMovementState::RUN;
}

bool AnimationManager::isCombatState(EntityMovementState state) const {
    return state == EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED ||
    state == EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED ||
    state == EntityMovementState::COMBAT_IDLE_ONE_HANDED ||
    state == EntityMovementState::COMBAT_SLASH_ONE_HANDED;
}

// TODO: create animation function for combat and for that another entity variable to cover the combats' counters
// TODO: make function animate only, implement all intervals elsewhere (repository for example)
void AnimationManager::animate(EntityMovementState state, real dt) {
    entity->setMovementState(state);
    int directionRow = entity->getMoveDirectionsSpritesMap()[entity->getMoveDirection()] - 1;
    int entityMovementStateColCount = entity->getMovementStateColCount(state);
    bool animate = false;
    real originScale = 0.5;
    real tileScale = 1;

    /*
    if (state == EntityMovementState::IDLE && entity->canAnimateIdle()) {
        animate = true;
    } else if (isMovementState(state)) {
        animate = entity->canAnimateMovement();
    } else if (isCombatState(state)) {
        animate = entity->canAttack();
        originScale = 1;
        tileScale = 2;
        // using the row generically
        directionRow += Constants::COMBAT_SLASH_ONE_HANDED_ROW * Constants::TILE_SIZE;
    }
     // TODO: use above variables to make function more generic
    */

    if (state == EntityMovementState::IDLE && entity->canAnimateIdle()) {
        entity->incrementMovementStateColCount(state);
        entity->setIntRectPosition(entityMovementStateColCount * Constants::TILE_SIZE,
                                   (entity->getMovementStateRowMap()[state] + directionRow) * Constants::TILE_SIZE,
                                   Constants::TILE_SIZE, Constants::TILE_SIZE);
        entity->getSprite()->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    } else if (isMovementState(state)) {
        if (entity->canAnimateMovement()) {
            entity->incrementMovementStateColCount(state);
            entity->setIntRectPosition(entityMovementStateColCount * Constants::TILE_SIZE,
                                    (entity->getMovementStateRowMap()[state] + directionRow) * Constants::TILE_SIZE,
                                    Constants::TILE_SIZE, Constants::TILE_SIZE);
            entity->getSprite()->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
        }
    } else if (isCombatState(state)) {
        if (entity->canAttack()) {
            entity->incrementMovementStateColCount(state);
            entity->setIntRectPosition(entityMovementStateColCount * Constants::TILE_SIZE * 2,
                                    // starting from the combat slash one handed row and jump each row by 128 pixels instead of regular 64
                                    Constants::COMBAT_SLASH_ONE_HANDED_ROW * Constants::TILE_SIZE +
                                    directionRow * Constants::TILE_SIZE * 2,
                                    Constants::TILE_SIZE * 2, Constants::TILE_SIZE * 2);

            entity->getSprite()->setOrigin(Constants::TILE_SIZE, Constants::TILE_SIZE);
            // triggering the timeout after swing was finished
            if (entityMovementStateColCount == Constants::MOVEMENT_STATE_NUM_COLS.at(state)-1) {
                entity->resetBattleIntervalForSwing();
            } else entity->resetBattleInterval();
        }
    } else if (state == EntityMovementState::JUMP) {
        if (entity->canAnimateJump()) {
            entity->incrementMovementStateColCount(state);
            entity->setIntRectPosition(entityMovementStateColCount * Constants::TILE_SIZE,
                                    (entity->getMovementStateRowMap()[state] + directionRow) * Constants::TILE_SIZE,
                                    Constants::TILE_SIZE, Constants::TILE_SIZE);
            entity->getSprite()->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
            // triggering the timeout after jump was finished
            if (entityMovementStateColCount == Constants::MOVEMENT_STATE_NUM_COLS.at(state)-1) {
                entity->resetJumpInterval();
            } else entity->resetJumpHeightSinceOnGroundInterval();
        }
    }
}

bool AnimationManager::generateBody() {
    for (std::string movementState : movementStates) {
        std::cout << movementState << std::endl;
        // using the generate_image.py script
        createImage("../graphics/Characters/Body/masculine/" + movementState + ".png",
                    "../graphics/Characters/Head/masculine/" + movementState + ".png",
                    movementState);
    }
    return true;
}

void AnimationManager::incrementCount(EntityMovementState state) {
    if (movementStateCounterMap[state] < Constants::MOVEMENT_STATE_NUM_COLS.at(state)-1) {
        movementStateCounterMap[state] = movementStateCounterMap[state] + 1;
        return;
    }
    movementStateCounterMap[state] = 0;
}


void AnimationManager::incrementIdleCount() {
    if (movementStateCounterMap[EntityMovementState::IDLE] < Constants::IDLE_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::IDLE] = movementStateCounterMap[EntityMovementState::IDLE] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::IDLE] = 0;
}

void AnimationManager::incrementClimbCount() {
    if (movementStateCounterMap[EntityMovementState::CLIMB] < Constants::CLIMB_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::CLIMB] = movementStateCounterMap[EntityMovementState::CLIMB] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::CLIMB] = 0;
}

void AnimationManager::incrementJumpCount() {
    if (movementStateCounterMap[EntityMovementState::JUMP] < Constants::JUMP_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::JUMP] = movementStateCounterMap[EntityMovementState::JUMP] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::JUMP] = 0;
}

void AnimationManager::incrementRunCount() {
    if (movementStateCounterMap[EntityMovementState::RUN] < Constants::RUN_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::RUN] = movementStateCounterMap[EntityMovementState::RUN] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::RUN] = 0;
}

void AnimationManager::incrementSittingCount() {
    if (movementStateCounterMap[EntityMovementState::SITTING] < Constants::SITTING_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::SITTING] = movementStateCounterMap[EntityMovementState::SITTING] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::SITTING] = 0;
}

void AnimationManager::incrementWalkCount() {
    if (movementStateCounterMap[EntityMovementState::WALK] < Constants::WALK_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::WALK] = movementStateCounterMap[EntityMovementState::WALK] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::WALK] = 0;
}

void AnimationManager::incrementCombatIdleOneHandedCount() {
    if (movementStateCounterMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] < Constants::COMBAT_IDLE_ONE_HANDED_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = movementStateCounterMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = 0;
}

void AnimationManager::incrementCombatSlashOneHandedCount() {
    if (movementStateCounterMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] < Constants::COMBAT_SLASH_ONE_HANDED_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = movementStateCounterMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = 0;
}

void AnimationManager::incrementCombatBackslashOneHandedCount() {
    if (movementStateCounterMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] < Constants::COMBAT_BACKSLASH_ONE_HANDED_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = movementStateCounterMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = 0;
}

void AnimationManager::incrementCombatHalfslashOneHandedCount() {
    if (movementStateCounterMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] < Constants::COMBAT_HALFSLASH_ONE_HANDED_NUM_COLS-1) {
        movementStateCounterMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = movementStateCounterMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] + 1;
        return;
    }
    movementStateCounterMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = 0;
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
