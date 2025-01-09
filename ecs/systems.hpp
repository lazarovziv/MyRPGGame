#pragma once

#include <SFML/Graphics.hpp>

#include "components.hpp"

#include "entt/entt.hpp"

#include <memory>
#include <string>

namespace mrg {

class Renderer {
  public:
    // a single renderer is required
    Renderer(const Renderer &) = delete;
    void operator=(const Renderer &) = delete;

    void render(std::unique_ptr<sf::RenderWindow> &window, std::unique_ptr<sf::View> &cameraView,
                const entt::registry &registry);
};

class Movement {
  public:
    Movement(const Movement &) = delete;
    void operator=(const Movement &) = delete;

    void move(entt::registry &registry, real dt);
};

} // namespace mrg