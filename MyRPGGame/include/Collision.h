#ifndef MYRPGGAME_COLLISION_H
#define MYRPGGAME_COLLISION_H

#include <SFML/Graphics.hpp>

namespace Collision {
    //////
    /// Test for a collision between two sprites by comparing the alpha values of overlapping pixels
    /// Supports scaling and rotation
    /// alphaLimit: The threshold at which a pixel becomes "solid". If alphaLimit is 127, a pixel with
    /// alpha value 128 will cause a collision and a pixel with alpha value 126 will not.
    ///
    /// This functions creates bitmasks of the textures of the two sprites by
    /// downloading the textures from the graphics card to memory -> SLOW!
    /// You can avoid this by using the "createTextureAndBitmask" function
    //////
    bool pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, sf::Uint8 alphaLimit = 0);

    ///////
    /// Test if a single pixel collides By testing the alpha value at the given location.
    /// Supports scaling and rotation
    /// alphaLimit: The threshold at which a pixel becomes "solid". If alphaLimit is 127, a pixel with
    /// alpha value 128 will cause a collision and a pixel with alpha value 126 will not.
    ///
    /// This functions creates bitmasks of the textures of the sprite by
    /// downloading the textures from the graphics card to memory -> SLOW!
    /// You can avoid this by using the "createTextureAndBitmask" function
    //////
    bool singlePixelTest(const sf::Sprite& sprite, sf::Vector2f& mousePosition, sf::Uint8 alphaLimit);

    //////
    /// Replaces Texture::loadFromFile
    /// Load an image file into the given texture and create a bitmask for it
    /// This is much faster than creating the bitmask for a texture on the first run of "pixelPerfectTest"
    ///
    /// The function returns false if the file could not be opened for some reason
    //////
    bool createTextureAndBitmask(sf::Texture &loadInto, const std::string& filename);

    //////
    /// Test for collision using circle collision detection
    /// Radius is averaged from the dimensions of the sprite so
    /// roughly circular objects will be much more accurate
    //////
    bool circleTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2);

    //////
    /// Test for bounding box collision using the Separating Axis Theorem
    /// Supports scaling and rotation
    //////
    bool boundingBoxTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
}

#endif //MYRPGGAME_COLLISION_H
