#ifndef AnimationManager_hpp
#define AnimationManager_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "GameEntity.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "ScriptExecutor.hpp"

using namespace std;

class AnimationManager {
private:
    GameEntity *entity;
    Weapon *weapon;

    // TODO: add constants for dimensions and all that kinda stuff
    map<AnimationPathType, string> animationsPaths;
    vector<string> bodyPaths;
    vector<string> clothingPaths;
    vector<string> headAccessoriesPaths;
    vector<string> hairPaths;
    
    vector<string> movementStates;
    // for calling the relevant function to increment step
    map<EntityMovementState, function<void()>> incrementMovementStateCountFunctionMap;
    // for storing and updating the step
    map<EntityMovementState, int> movementStateCounterMap;

    int idleCount, climbCount, combatBackslashOneHandedCount, combatHalfslashOneHandedCount,
    combatIdleOneHandedCount, combatSlashOneHandedCount, runCount, sittingCount, walkCount, jumpCount;

public:
    AnimationManager(GameEntity *entity);
    ~AnimationManager() = default;

    void addBodyPath(string path);
    void addClothingPath(string path);
    void addHeadAccessoriesPath(string path);
    void addHairPath(string path);

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

    bool generateBody();
    bool generateBodyTest();
    bool generateClothing();
    // TODO: add more generate functions

    void animate(EntityMovementState state, float dt);

};

#endif