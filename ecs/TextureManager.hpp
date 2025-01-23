#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <memory>
#include <string>

namespace rg {

class TextureManager {
  private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> texturesMap;

    static TextureManager* instance;

  public:
    TextureManager(const TextureManager&) = delete;
    void operator=(const TextureManager&) = delete;

    static TextureManager* getInstance();

    void insertTexture(const std::string& path);
    std::shared_ptr<sf::Texture> retrieveTexture(const std::string& path);

  private:
    TextureManager() = default;
    ~TextureManager();
};

} // namespace rg
