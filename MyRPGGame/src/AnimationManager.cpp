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
    COMBAT_IDLE_ONE_HANDED, COMBAT_SLASH_ONE_HANDED, WALK, JUMP, SITTING, RUN, IDLE };
*/

bool AnimationManager::isMovementState(const EntityMovementState state) const {
    return state == EntityMovementState::WALK || state == EntityMovementState::RUN;
}

bool AnimationManager::isCombatState(const EntityMovementState state) const {
    return state == EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED ||
    state == EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED ||
    state == EntityMovementState::COMBAT_IDLE_ONE_HANDED ||
    state == EntityMovementState::COMBAT_SLASH_ONE_HANDED;
}

// TODO: create animation function for combat and for that another entity variable to cover the combats' counters
// TODO: make function animate only, implement all intervals elsewhere (repository for example)
// TODO: move all unnecessary logic from animate method to another function which is not in the animation manager, i.e, make animation manager manage animation ONLY
void AnimationManager::animate(const EntityMovementState state, const real dt) {
    entity->setMovementState(state);
    // using the row generically
    int directionRow = entity->getMoveDirectionsSpritesMap()[entity->getMoveDirection()] - 1;
    int entityMovementStateColCount = entity->getMovementStateColCount(state);
    bool isLastColumn = entityMovementStateColCount == Constants::MOVEMENT_STATE_NUM_COLS.at(state) - 1;
    bool animate = false;
    real originScale = 0.5;
    int tileScale = 1;
    // function pointers for after the animation has finished (one sprite of it)
    void (GameEntity::*finishFunction)() = nullptr;
    void (GameEntity::*nonFinishFunction)() = nullptr;

    if (state == EntityMovementState::IDLE) {
        animate = entity->canAnimateIdle();
        directionRow += entity->getMovementStateRowMap()[state];
    } else if (isMovementState(state)) {
        animate = entity->isMoving() && entity->canAnimateMovement();
        directionRow += entity->getMovementStateRowMap()[state];
        finishFunction = &GameEntity::resetMoving;
    } else if (isCombatState(state)) {
        animate = entity->isAttacking() && entity->canAttack();
        originScale = 1;
        tileScale = 2;
        directionRow *= tileScale;
        directionRow += Constants::COMBAT_SLASH_ONE_HANDED_ROW;
        finishFunction = &GameEntity::resetAttacking;
        nonFinishFunction = &GameEntity::resetBattleInterval;
    } else if (state == EntityMovementState::JUMP) {
        animate = entity->isJumping() && entity->canAnimateJump();
        directionRow += entity->getMovementStateRowMap()[state];
        finishFunction = &GameEntity::resetJumping;
        nonFinishFunction = &GameEntity::resetJumpHeightSinceOnGroundInterval;
    }

    if (animate) {
        entity->incrementMovementStateColCount(state);
        entity->setIntRectPosition(entityMovementStateColCount * Constants::TILE_SIZE * tileScale,
                                               directionRow * Constants::TILE_SIZE,
                                               Constants::TILE_SIZE * tileScale,
                                               Constants::TILE_SIZE * tileScale);
        entity->getSprite()->setOrigin(Constants::TILE_SIZE * originScale, Constants::TILE_SIZE * originScale);
        // reached end of the entire animation
        if (isLastColumn && finishFunction != nullptr) {
            (*entity.*finishFunction)();
        } else if (nonFinishFunction != nullptr) (*entity.*nonFinishFunction)();
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

void AnimationManager::incrementCount(const EntityMovementState state) {
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
