#include "entt/entt.hpp"

#include "Constants.h"
#include "components.hpp"
#include "window.hpp"
#include "simulation.hpp"

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

    prepareSimulation(registry);

    entt::entity entity = registry.create();
    auto &transform = registry.emplace<mrg::Transform>(entity);
    const std::string &texturePath = "../../../graphics/player/gray_t_shirt_leather_pants_spritesheet.png";
    auto &renderable = registry.emplace<mrg::Renderable>(entity, texturePath);
    transform.position = glm::vec3{mrg::Constants::WINDOW_WIDTH / 2, mrg::Constants::WINDOW_HEIGHT + 100, 0};
    auto &moveable = registry.emplace<mrg::Moveable>(entity);
    auto &rigidBody = registry.emplace<mrg::RigidBody>(entity, 0.2);

    entt::entity first = registry.create();
    auto &t = registry.emplace<mrg::Transform>(first);
    const std::string &tPath = "../../../graphics/player/uriel_spritesheet.png";
    auto &ren = registry.emplace<mrg::Renderable>(first, tPath);
    t.position = glm::vec3{mrg::Constants::WINDOW_WIDTH / 2 + 200, mrg::Constants::WINDOW_HEIGHT, 0};
    auto &m = registry.emplace<mrg::Moveable>(first);
    auto &r = registry.emplace<mrg::RigidBody>(first, 1);

    mrg::Movement movement{};
    mrg::Physics physics{};

    const std::string &title = "Test";
    mrg::Window window{mrg::Constants::WINDOW_WIDTH, mrg::Constants::WINDOW_HEIGHT, title};
    sf::Event event;

    bool running = window.isOpen();

    real dt = 0;

    while (running) {
        while (window.pollingEvents(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            rigidBody.addForce(glm::vec3{0, -mrg::Constants::FORCE_SCALAR, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            rigidBody.addForce(glm::vec3{0, mrg::Constants::FORCE_SCALAR, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            rigidBody.addForce(glm::vec3{mrg::Constants::FORCE_SCALAR, 0, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            rigidBody.addForce(glm::vec3{-mrg::Constants::FORCE_SCALAR, 0, 0});
        }

        auto startTime = std::chrono::high_resolution_clock::now();

		movement.update(registry, dt);
        physics.update(registry, dt);

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<real, std::chrono::seconds::period>(stopTime - startTime).count();

        window.render(registry);
    }

    return EXIT_SUCCESS;
}
