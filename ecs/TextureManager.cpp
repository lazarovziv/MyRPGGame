#include "TextureManager.hpp"

#include <stdexcept>

namespace rg {
    TextureManager* TextureManager::instance{nullptr};

    TextureManager* TextureManager::getInstance() {
        if (instance) {
            return instance;
        }
        instance = new TextureManager();
        return instance;
    }

    TextureManager::~TextureManager() {
        delete instance;
        instance = nullptr;
    }

    void TextureManager::insertTexture(const std::string& path) {
        if (texturesMap.find(path) != texturesMap.end()) {
            throw std::runtime_error("Texture already exists! [" + path + "]");
        }
        // texture doesn't exist so we'll create it and insert into the map
        sf::Texture texture;
        if (!texture.loadFromFile(path)) throw std::runtime_error("Texture was NOT loaded properly! [" + path + "]");
        texture.setSmooth(true);

        texturesMap.insert({path, std::make_shared<sf::Texture>(texture)});
    }

    std::shared_ptr<sf::Texture> TextureManager::retrieveTexture(const std::string& path) {
        // first time using the image as texture so we'll insert it
        if (texturesMap.find(path) == texturesMap.end()) {
            insertTexture(path);
        }
        return texturesMap[path];
    }
}
