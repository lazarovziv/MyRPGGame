#ifndef AnimationManager_hpp
#define AnimationManager_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include "GameEntity.hpp"
//#include <opencv2/core/mat.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
#include "ScriptExecutor.hpp"

class AnimationManager {
private:
    // TODO: remove entity field and pass weak_ptr to every method that uses entity
    GameEntity *entity;

    // TODO: add constants for dimensions and all that kinda stuff
    std::map<AnimationPathType, std::string> animations_paths;
    std::vector<std::string> body_paths;
    std::vector<std::string> clothing_paths;
    std::vector<std::string> head_accessories_paths;
    std::vector<std::string> hair_paths;
    
    std::vector<std::string> movement_states;
    // for calling the relevant function to increment step
    std::map<EntityMovementState, std::function<void()>> increment_movement_state_count_function_map;
    // for storing and updating the step
    std::map<EntityMovementState, int> movement_state_counter_map;

    int idle_count, climb_count, combat_backslash_one_handed_count, combat_halfslash_one_handed_count,
    combat_idle_one_handed_count, combat_slash_one_handed_count, run_count, sitting_count, walk_count, jump_count;

public:
    AnimationManager(GameEntity *entity);
    ~AnimationManager() = default;

    void add_body_path(std::string path);
    void add_clothing_path(std::string path);
    void add_head_accessories_path(std::string path);
    void add_hair_path(std::string path);

    void set_entity(GameEntity *entity);

    int get_movement_state_count(EntityMovementState state);

    void increment_count(EntityMovementState state);

    bool is_movement_state(EntityMovementState state) const;
    bool is_combat_state(EntityMovementState state) const;

    bool generate_body();
    bool generate_body_test();
    bool generate_clothing();
    // TODO: add more generate functions

    void animate(EntityMovementState state, real dt);

};

#endif