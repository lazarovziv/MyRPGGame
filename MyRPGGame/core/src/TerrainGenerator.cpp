#include "TerrainGenerator.hpp"

TerrainGenerator::TerrainGenerator() {}

real TerrainGenerator::interpolate(real a0, real a1, real w) {
    if (w < 0.f) return a0;
    if (w > 1.f) return a1;
    // cubic interpolation for smoother results
//    return (a1 - a0) * (3.f - w * 2.f) * w * w + a0;
    // smoother?
    return (a1 - a0) * ((w * (w * 6.0 - 15.f) + 10.f) * w * w * w) + a0;
}

sf::Vector2f TerrainGenerator::random_gradient(int positionX, int positionY) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w >> 1;
    unsigned a = positionX, b = positionY;
    a *= 3284157443;
    b ^= a << s | a >> (w-s);
    b *= 1911520717;
    a ^= b << s | b >> (w-s);
    a *= 2048419325;
    real random = a * (3.14159265 / ~(~0u >> 1)); // keep it in range of [0, 2 pi]
    sf::Vector2f vector;
    vector.x = cos(random);
    vector.y = sin(random);
    return vector;
}

real TerrainGenerator::dot_grid_gradient(int positionX, int positionY, real x, real y) {
    // get gradient from position on grid
    sf::Vector2f gradient = random_gradient(positionX, positionY);
    // calculate distance vector
    real dx = x - (real) positionX;
    real dy = y - (real) positionY;
    // return dot product
    return (dx * gradient.x + dy * gradient.y);
}

real TerrainGenerator::perlin(real x, real y) {
    // get grid coordinates
    int x0 = (int) floor(x);
    int x1 = x0 + 1;
    int y0 = (int) floor(y);
    int y1 = y0 + 1;
    // calculate interpolation weights
    real sx = x - (real) x0;
    real sy = y - (real) y0;
    // interpolate
    real n0 = dot_grid_gradient(x0, y0, x, y);
    real n1 = dot_grid_gradient(x1, y0, x, y);
    real ix0 = interpolate(n0, n1, sx);
    n0 = dot_grid_gradient(x0, y1, x, y);
    n1 = dot_grid_gradient(x1, y1, x, y);
    real ix1 = interpolate(n0, n1, sx);

    return interpolate(ix0, ix1, sy); // getting value in range [-1, 1]
}

sf::Uint8* TerrainGenerator::generate(int width, int height) {
    // creating 3d array
    auto grid = new sf::Uint8[width * height * 4];
    real value, freq, amp;
    int finalColor;
    int index;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            index = (y * width + x) * 4;
            // resetting values for current iteration
            value = 0;
            freq = 1;
            amp = 1;

            for (int i = 0; i < 12; i++) {
                value += perlin(x * freq / 64, y * freq / 64) * amp;
                freq *= 2;
                amp /= 2;
            }
            // for contrast
            value *= 1.2f;
            // fixing values to range
            if (value > 1.f) value = 1.f;
            else if (value < -1.f) value = -1.f;

            finalColor = (int) ((value + 1.f) * 0.5f * 255);
            for (int color = 0; color < 3; color++) {
                grid[index] = finalColor;
            }
            grid[index + 3] = 255;
        }
    }
    return grid;
}

/*
    TerrainGenerator generator;
    sf::Uint8* grid = generator.generate(800, 600);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Perlin Test");
    sf::Texture texture;
    sf::Sprite sprite;
    texture.create(800, 600);
    texture.update(grid);
    sprite.setTexture(texture);
 */
