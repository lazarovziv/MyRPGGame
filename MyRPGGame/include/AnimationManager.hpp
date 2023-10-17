#ifndef AnimationManager_hpp
#define AnimationManager_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include "GameEntity.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "ScriptExecutor.hpp"

class AnimationManager {
private:
    GameEntity *entity;
    Weapon *weapon;

    // TODO: add constants for dimensions and all that kinda stuff
    std::map<AnimationPathType, std::string> animationsPaths;
    std::vector<std::string> bodyPaths;
    std::vector<std::string> clothingPaths;
    std::vector<std::string> headAccessoriesPaths;
    std::vector<std::string> hairPaths;
    
    std::vector<std::string> movementStates;
    // for calling the relevant function to increment step
    std::map<EntityMovementState, std::function<void()>> incrementMovementStateCountFunctionMap;
    // for storing and updating the step
    std::map<EntityMovementState, int> movementStateCounterMap;

    int idleCount, climbCount, combatBackslashOneHandedCount, combatHalfslashOneHandedCount,
    combatIdleOneHandedCount, combatSlashOneHandedCount, runCount, sittingCount, walkCount, jumpCount;

public:
    AnimationManager(GameEntity *entity);
    ~AnimationManager() = default;

    void addBodyPath(std::string path);
    void addClothingPath(std::string path);
    void addHeadAccessoriesPath(std::string path);
    void addHairPath(std::string path);

    void setEntity(GameEntity *entity);

    int getMovementStateCount(EntityMovementState state);

    void incrementCount(EntityMovementState state);

    void incrementIdleCount();
    void incrementClimbCount();
    void incrementCombatBackslashOneHandedCount();
    void incrementCombatHalfslashOneHandedCount();
    void incrementCombatIdleOneHandedCount();
    void incrementCombatSlashOneHandedCount();
    void incrementRunCount();
    void incrementSittingCount();
    void incrementWalkCount();
    void incrementJumpCount();

    bool isMovementState(EntityMovementState state) const;
    bool isCombatState(EntityMovementState state) const;

    bool generateBody();
    bool generateBodyTest();
    bool generateClothing();
    // TODO: add more generate functions

    void animate(EntityMovementState state, real dt);

};

#endif