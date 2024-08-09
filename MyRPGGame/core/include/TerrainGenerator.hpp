#ifndef MYRPGGAME_TERRAINGENERATOR_HPP
#define MYRPGGAME_TERRAINGENERATOR_HPP

#include <SFML/Graphics.hpp>
#include <math.h>
#include "Constants.h"

class TerrainGenerator {
private:

public:
    TerrainGenerator();
    ~TerrainGenerator() = default;

    real interpolate(real a0, real a1, real w);
    sf::Vector2f random_gradient(int position_x, int position_y);
    real dot_grid_gradient(int position_x, int position_y, real x, real y);
    real perlin(real x, real y);

    sf::Uint8* generate(int width, int height);

};

#endif //MYRPGGAME_TERRAINGENERATOR_HPP
