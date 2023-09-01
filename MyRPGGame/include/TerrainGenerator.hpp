#ifndef MYRPGGAME_TERRAINGENERATOR_HPP
#define MYRPGGAME_TERRAINGENERATOR_HPP

#include <SFML/Graphics.hpp>
#include <math.h>

class TerrainGenerator {
private:

public:
    TerrainGenerator();
    ~TerrainGenerator() = default;

    float interpolate(float a0, float a1, float w);
    sf::Vector2f randomGradient(int positionX, int positionY);
    float dotGridGradient(int positionX, int positionY, float x, float y);
    float perlin(float x, float y);

    sf::Uint8* generate(int width, int height);

};

#endif //MYRPGGAME_TERRAINGENERATOR_HPP
