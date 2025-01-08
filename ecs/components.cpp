#include "components.hpp"

namespace mrg {

Renderable::Renderable(const std::string &texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Texture was NOT loaded properly!\n";
        texture.setSmooth(true);
    } else std::cerr << "Texture was loaded properly.\n";
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 33 * Constants::TILE_SIZE,
                                      Constants::TILE_SIZE, Constants::TILE_SIZE));
    sprite.setOrigin(Constants::TILE_SIZE / 2, Constants::TILE_SIZE / 2);
}

void Renderable::setPosition(const glm::vec3 &position) {
    sprite.setPosition(position.x, position.y);
}

} // namespace mrg