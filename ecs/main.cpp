#include "entt/entt.hpp"

#include "Constants.h"
#include "components.hpp"
#include "simulation.hpp"
#include "window.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

// std
#include <iostream>
#include <random>
#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    entt::registry registry;

    prepareSimulation(registry);

    entt::entity entity = registry.create();
    auto& transform = registry.emplace<rg::Transform>(entity);
    const std::string& texturePath =
        rg::Constants::getGraphicsPath("graphics/player/gray_t_shirt_leather_pants_spritesheet.png");
    auto& renderable = registry.emplace<rg::Renderable>(entity, texturePath);
    transform.position = glm::vec3{rg::Constants::WINDOW_WIDTH / 2,
                                   rg::Constants::WINDOW_HEIGHT + 100, 0};
    auto& moveable = registry.emplace<rg::Moveable>(entity);
    auto& rigidBody = registry.emplace<rg::RigidBody>(entity, 0.2);

    entt::entity first = registry.create();
    auto& t = registry.emplace<rg::Transform>(first);
    const std::string& tPath = rg::Constants::getGraphicsPath("graphics/player/uriel_spritesheet.png");
    auto& ren = registry.emplace<rg::Renderable>(first, tPath);
    t.position = glm::vec3{rg::Constants::WINDOW_WIDTH / 2 + 200,
                           rg::Constants::WINDOW_HEIGHT, 0};
    auto& m = registry.emplace<rg::Moveable>(first);
    auto& r = registry.emplace<rg::RigidBody>(first, 1);

    rg::Movement movement{};
    rg::Physics physics{};

    const std::string& title = "Test";
    rg::Window window{rg::Constants::WINDOW_WIDTH, rg::Constants::WINDOW_HEIGHT,
                      title};
    sf::Event event;

    bool running = window.isOpen();

    // TODO: move to a dedicated class
    glm::vec3 direction{0, 0, 0};
    auto upDirectionVector = glm::vec3{0, -rg::Constants::FORCE_SCALAR, 0};
    auto downDirectionVector = glm::vec3{0, rg::Constants::FORCE_SCALAR, 0};
    auto rightDirectionVector = glm::vec3{rg::Constants::FORCE_SCALAR, 0, 0};
    auto leftDirectionVector = glm::vec3{-rg::Constants::FORCE_SCALAR, 0, 0};

    real dt = 0;

    while (running) {
        while (window.pollingEvents(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            direction += upDirectionVector;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            direction += downDirectionVector;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            direction += rightDirectionVector;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            direction += leftDirectionVector;
        }

        rigidBody.addForce(direction);

        auto startTime = std::chrono::high_resolution_clock::now();

        movement.update(registry, dt);
        physics.update(registry, dt);

        auto stopTime = std::chrono::high_resolution_clock::now();

        dt = std::chrono::duration<real, std::chrono::seconds::period>(
                 stopTime - startTime)
                 .count();

        window.render(transform.position, registry);

        direction.x = 0;
        direction.y = 0;
        direction.z = 0;
    }

    return EXIT_SUCCESS;
}
