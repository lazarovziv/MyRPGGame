#include "entt/entt.hpp"

#include "Constants.h"
#include "components.hpp"
#include "window.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

// std
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    entt::registry registry;
    // creating entities
    entt::entity first = registry.create();
    entt::entity entity = registry.create();

    auto &transform = registry.emplace<mrg::Transform>(entity);
    const std::string &texturePath = "../../../graphics/player/uriel_spritesheet.png";
    auto &renderable = registry.emplace<mrg::Renderable>(entity, texturePath);
    transform.position = glm::vec3{mrg::Constants::WINDOW_WIDTH / 2, mrg::Constants::WINDOW_HEIGHT, 0};

    auto &firstTransform = registry.emplace<mrg::Transform>(first);
    const std::string &firstTexturePath = "../../../graphics/player/gray_t_shirt_leather_pants_spritesheet.png";
    auto &firstRenderable = registry.emplace<mrg::Renderable>(first, firstTexturePath);
    firstTransform.position = glm::vec3{mrg::Constants::WINDOW_WIDTH / 2 + 1 * mrg::Constants::TILE_SIZE,
                                        mrg::Constants::WINDOW_HEIGHT + 1 * mrg::Constants::TILE_SIZE, 0};

    const std::string &title = "Test";
    mrg::Window window{mrg::Constants::WINDOW_WIDTH, mrg::Constants::WINDOW_HEIGHT, title};
    sf::Event event;

    bool running = window.isOpen();

    while (running) {
        while (window.pollingEvents(event)) {
            // if (event.type == sf::Event::Closed) {
            //     running = false;
            // }
        }
        window.render(registry);
    }

    return EXIT_SUCCESS;
}