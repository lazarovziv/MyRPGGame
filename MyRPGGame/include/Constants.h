#ifndef Constants_h
#define Constants_h

#include <iostream>
#include <string>
#include <map>
#include <limits>
#include "math.h"
#include <vector>

// for using throughout the project, if want to change to int/double
typedef float real;

enum class MoveDirection { DOWN, RIGHT, LEFT, UP, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT };

enum class WeaponType { SWORD, DAGGER, STAFF, AXE, MACE, BARE_HANDED };

enum class AnimationPathType { HEAD, BODY, TORSO, PANTS, WEAPON };

enum class EntityMovementState {
    CLIMB, COMBAT_BACKSLASH_ONE_HANDED, COMBAT_HALFSLASH_ONE_HANDED,
    COMBAT_IDLE_ONE_HANDED, COMBAT_SLASH_ONE_HANDED, WALK, JUMP, SITTING, RUN, IDLE };

class Constants {
public:
    // for the physics collision detection (impacts performance but more precise)
    static const int UPDATE_ITERATIONS = 1;

    static constexpr real SCREEN_WIDTH = 800;
    static constexpr real SCREEN_HEIGHT = 640;
    static constexpr real FULL_SCREEN_WIDTH = 918;//4 * SCREEN_WIDTH;
    static constexpr real FULL_SCREEN_HEIGHT = 515; //3 * SCREEN_HEIGHT;
    static constexpr real TILE_SIZE = 64;
    static constexpr real BASE_ENTITY_SPEED = 6 * UPDATE_ITERATIONS;
    static const int FPS = 60;

    static constexpr real REAL_MAX = std::numeric_limits<real>::max();
    static constexpr real REAL_MIN = std::numeric_limits<real>::min();

    static constexpr real RIGID_BODY_FORCE_SCALE = (real) 64;
    static constexpr real VELOCITY_MAGNITUDE_MIN = 0.002;
    static constexpr real VELOCITY_MAGNITUDE_MAX = BASE_ENTITY_SPEED * 0.45;
    // TODO: add more friction variables per ground type
    static constexpr real ICE_FRICTION_DEGRADATION_CONSTANT = (real) 1.0015;
    static constexpr real GRASS_FRICTION_DEGRADATION_CONSTANT = (real) 1.005;

    static const int NUM_FRAMES_IDLE_ANIMATION = 13;

    static const int NUM_ROWS = 3;
    static const int NUM_COLS = 3;
    static const int NUM_STEPS = 8;
    static const int WEAPON_TRANSITION_MIN = 4;
    static const int WEAPON_TRANSITION_MAX = 7;

    // constants for the movement animations
    static const int IDLE_NUM_COLS = 3;
    static const int IDLE_NUM_ROWS = 4;
    static const int IDLE_ROW = 17; // start of the sprite sheet
    static const int JUMP_NUM_COLS = 6;
    static const int JUMP_NUM_ROWS = 4;
    static const int JUMP_ROW = 21;
    static const int RUN_NUM_COLS = 8;
    static const int RUN_NUM_ROWS = 4;
    static const int RUN_ROW = 25;
    static const int SITTING_NUM_COLS = 3;
    static const int SITTING_NUM_ROWS = 4;
    static const int SITTING_ROW = 29;
    static const int WALK_NUM_COLS = 8;
    static const int WALK_NUM_ROWS = 4;
    static const int WALK_ROW = 33;
    // TODO: define combat rows from zero where first row is the first occurrence of 128x128 pixels
    // OR calculate rows by the 128x128 rows above them
    static const int COMBAT_IDLE_ONE_HANDED_NUM_COLS = 2;
    static const int COMBAT_IDLE_ONE_HANDED_NUM_ROWS = 4;
    static const int COMBAT_IDLE_ONE_HANDED_ROW = 9;
    static const int COMBAT_SLASH_ONE_HANDED_NUM_COLS = 6;
    static const int COMBAT_SLASH_ONE_HANDED_NUM_ROWS = 4;
    static const int COMBAT_SLASH_ONE_HANDED_ROW = 37; //13;
    static const int COMBAT_BACKSLASH_ONE_HANDED_NUM_COLS = 12;
    static const int COMBAT_BACKSLASH_ONE_HANDED_NUM_ROWS = 4;
    static const int COMBAT_BACKSLASH_ONE_HANDED_ROW = 1;
    static const int COMBAT_HALFSLASH_ONE_HANDED_NUM_COLS = 6;
    static const int COMBAT_HALFSLASH_ONE_HANDED_NUM_ROWS = 4;
    static const int COMBAT_HALFSLASH_ONE_HANDED_ROW = 5;
    static const int CLIMB_NUM_COLS = 6;
    static const int CLIMB_NUM_ROWS = 1;
    static const int CLIMB_ROW = 0;

    inline static const std::map<EntityMovementState, int> MOVEMENT_STATE_NUM_COLS = {
            { EntityMovementState::CLIMB, Constants::CLIMB_NUM_COLS },
            { EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED, Constants::COMBAT_BACKSLASH_ONE_HANDED_NUM_COLS },
            { EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED, Constants::COMBAT_HALFSLASH_ONE_HANDED_NUM_COLS },
            { EntityMovementState::COMBAT_IDLE_ONE_HANDED, Constants::COMBAT_IDLE_ONE_HANDED_NUM_COLS },
            { EntityMovementState::COMBAT_SLASH_ONE_HANDED, Constants::COMBAT_SLASH_ONE_HANDED_NUM_COLS },
            { EntityMovementState::WALK, Constants::WALK_NUM_COLS },
            { EntityMovementState::JUMP, Constants::JUMP_NUM_COLS },
            { EntityMovementState::SITTING, Constants::SITTING_NUM_COLS },
            { EntityMovementState::RUN, Constants::RUN_NUM_COLS },
            { EntityMovementState::IDLE, Constants::IDLE_NUM_COLS }
    };

    inline static const std::map<EntityMovementState, int> MOVEMENT_STATE_ROW = {
            { EntityMovementState::CLIMB, Constants::CLIMB_ROW },
            { EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED, Constants::COMBAT_BACKSLASH_ONE_HANDED_ROW },
            { EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED, Constants::COMBAT_HALFSLASH_ONE_HANDED_ROW },
            { EntityMovementState::COMBAT_IDLE_ONE_HANDED, Constants::COMBAT_IDLE_ONE_HANDED_ROW },
            { EntityMovementState::COMBAT_SLASH_ONE_HANDED, Constants::COMBAT_SLASH_ONE_HANDED_ROW },
            { EntityMovementState::WALK, Constants::WALK_ROW },
            { EntityMovementState::JUMP, Constants::JUMP_ROW },
            { EntityMovementState::SITTING, Constants::SITTING_ROW },
            { EntityMovementState::RUN, Constants::RUN_ROW },
            { EntityMovementState::IDLE, Constants::IDLE_ROW }
    };

    inline static const std::vector<EntityMovementState> COMBAT_STATES = {
            EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED, EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED,
            EntityMovementState::COMBAT_IDLE_ONE_HANDED, EntityMovementState::COMBAT_SLASH_ONE_HANDED
    };

    // inline is a c++ 17 feature
    inline static const std::string PLAYER_IMAGES_PATH = "../graphics/player/body/";

    // enum for distinguishing moves - CHANGE_* values means needs to change to relevant map
    enum MoveSuccessValues {
        NOT_MOVED, FAILURE, SUCCESS, CHANGE_UP, CHANGE_DOWN, CHANGE_RIGHT, CHANGE_LEFT
    };

    // enum AttackSuccessValues {
    //     FAILURE, SUCCESS
    // };

    enum class GameState { PLAYING, EXITING, RESUMING, IN_MENU };
};

#endif /* Constants_h */