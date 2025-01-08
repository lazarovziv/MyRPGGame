#pragma once

#include <stdint.h>

typedef float real;

namespace mrg {

class Constants {
    public:
        static constexpr uint32_t TILE_SIZE = 64;
        static constexpr uint32_t WINDOW_WIDTH = 800;
        static constexpr uint32_t WINDOW_HEIGHT = 600;
};

} // namespace mrg
