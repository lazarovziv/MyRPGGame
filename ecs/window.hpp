#pragma once

#include <SFML/Graphics.hpp>

#include "entt/entt.hpp"

#include "Constants.h"
#include "systems.hpp"

#include <memory>
#include <string>

namespace mrg {

class Window {
  private:
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::View> cameraView;

    Renderer renderer{};

  public:
    Window(const uint32_t width, const uint32_t height, const std::string &title);
    Window(const Window &) = delete;
    void operator=(const Window &) = delete;

    void render(entt::registry &registry);
    bool isOpen() { return window->isOpen();}
    bool pollingEvents(sf::Event &event) { return window->pollEvent(event); };
};

} // namespace mrg