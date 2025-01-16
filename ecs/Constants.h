#pragma once

#include <stdint.h>
#include <limits>

typedef float real;

namespace mrg {

class Constants {
    public:
        static constexpr real TILE_SIZE = 64;
        static constexpr uint32_t WINDOW_WIDTH = 800;
        static constexpr uint32_t WINDOW_HEIGHT = 600;

        static constexpr real BASE_ENTITY_SPEED = 4;

        static constexpr real RIGID_BODY_FORCE_SCALE = (real) 64;
        static constexpr real VELOCITY_MAGNITUDE_MIN = 0.002;
        static constexpr real VELOCITY_MAGNITUDE_MAX = BASE_ENTITY_SPEED * 0.45;
        // TODO: add more friction variables per ground type
        static constexpr real ICE_FRICTION_DEGRADATION_CONSTANT = (real) 1.0015;
        static constexpr real GRASS_FRICTION_DEGRADATION_CONSTANT = (real) 1.005;

        static constexpr real INFINITE_MASS_MASS_VALUE = 0;

        static constexpr real FORCE_SCALAR = 100000; // 1000000;

        static constexpr real REAL_MAX = std::numeric_limits<real>::max();
        static constexpr real REAL_MIN = std::numeric_limits<real>::min();

        enum class SpriteMoveDirection {
            TOP = 0, LEFT = 1, BOTTOM = 2, RIGHT = 3
        };

};

} // namespace mrg
