#pragma once

#include <limits>
#include <string>
#include <cstdint>

typedef float real;

namespace rg {

class Constants {
  public:
    static constexpr real TILE_SIZE = 64;
    static constexpr uint32_t WINDOW_WIDTH = 800;
    static constexpr uint32_t WINDOW_HEIGHT = 600;

    static constexpr real BASE_ENTITY_SPEED = 4;

    static constexpr real RIGID_BODY_FORCE_SCALE = 64.0;
    static constexpr real VELOCITY_MAGNITUDE_MIN = 0.002;
    static constexpr real VELOCITY_MAGNITUDE_MAX = BASE_ENTITY_SPEED * 0.45;
    // TODO: add more friction variables per ground type
    static constexpr real ICE_FRICTION_DEGRADATION_CONSTANT = 1.0015;
    static constexpr real GRASS_FRICTION_DEGRADATION_CONSTANT = 1.005;

    static constexpr real INFINITE_MASS_MASS_VALUE = 0;

    static constexpr real FORCE_SCALAR = 100000 / 8; // 1000000;

    static constexpr real REAL_MAX = std::numeric_limits<real>::max();
    static constexpr real REAL_MIN = std::numeric_limits<real>::min();

    enum class EntityActionState {
        CLIMB,
        COMBAT_BACKSLASH_ONE_HANDED,
        COMBAT_HALFSLASH_ONE_HANDED,
        COMBAT_IDLE_ONE_HANDED,
        COMBAT_SLASH_ONE_HANDED,
        WALK,
        JUMP,
        SIT,
        RUN,
        IDLE
    };

    enum class SpriteMoveDirection { TOP = 0, LEFT = 1, BOTTOM = 2, RIGHT = 3 };

private:
    // string can't be constexpr so we're using string_view
    static constexpr std::string_view BASE_GRAPHICS_DIR = "../../../../";
    static constexpr std::string_view BASE_RESOURCE_DIR = "../../../";

public:
    static std::string getGraphicsPath(const std::string& path) {
        return std::string{BASE_GRAPHICS_DIR} + path;
    }

    static std::string getResourcePath(const std::string& path) {
        return std::string{BASE_RESOURCE_DIR} + path;
    }
};

} // namespace rg
