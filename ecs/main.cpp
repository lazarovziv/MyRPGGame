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

struct ComponentA {
    real x;
    real y;
};

struct ComponentB {
    real velocity;
    real acceleration;
};

struct ComponentC {
    real value;
};

struct ComponentD {
    int num;
};

int main() {
    entt::registry registry;
    entt::entity first = registry.create();
    entt::entity entity = registry.create();

    auto &entityComponentA = registry.emplace<ComponentA>(entity);
    entityComponentA.x = 1.0;
    entityComponentA.y = 2.0;

    auto &entityComponentB = registry.emplace<ComponentB>(entity);
    entityComponentB.velocity = 0.5;
    entityComponentB.acceleration = 0.25;

    auto &firstComponentA = registry.emplace<ComponentA>(first);
    firstComponentA.x = 3.0;
    firstComponentA.y = 4.0;

    auto &firstComponentB = registry.emplace<ComponentB>(first);
    firstComponentB.velocity = 0.6;
    firstComponentB.acceleration = 0.35;

    // using the existing component
    registry.patch<ComponentA>(entity, [](auto &a) {
        a.x = 800.0;
        a.y = 1000.0;
    });

    // creating a new component altogether
    // registry.patch<ComponentA>(entity, (real) 900.0, (real) 1100.0);

    // add ComponentC to all entities with ComponentA and ComponentB
    auto viewAB = registry.view<ComponentA, ComponentB>();
    // std::vector componentsC = { ComponentC{16.0}, ComponentC{24.0}};
    // registry.insert<ComponentC>(viewAB.begin(), viewAB.end(), &componentsC);
    registry.insert<ComponentC>(viewAB.begin(), viewAB.end(), {16.0});

    ComponentA &entityA = registry.get<ComponentA>(entity);
    // if we don't know whether entity has ComponentC, we'll use try_get
    ComponentC &entityC = registry.get<ComponentC>(entity);

    std::cout << "(" << entityA.x << ", " << entityA.y << ")" << '\n';
    std::cout << entityC.value << '\n';
    std::cout << registry.get<ComponentC>(first).value << '\n';

    registry.emplace_or_replace<ComponentD>(entity, 1);
    std::cout << registry.get<ComponentD>(entity).num << '\n';

    // deleting a component when certain it exists
    registry.erase<ComponentB>(first);
    // if not, use remove

    // true if entity has all the given components
    bool all = registry.all_of<ComponentA, ComponentB>(entity);
    // true if entity has at least one of the given components
    bool any = registry.any_of<ComponentA, ComponentC>(first);

    auto &transform = registry.emplace<mrg::Transform>(entity);
    const std::string &texturePath = "../../../graphics/player/uriel_spritesheet.png";
    auto &renderable = registry.emplace<mrg::Renderable>(entity, texturePath);
    transform.position = glm::vec3{mrg::Constants::WINDOW_WIDTH / 2, mrg::Constants::WINDOW_HEIGHT, 0};

    const std::string &title = "Test";
    mrg::Window window{mrg::Constants::WINDOW_WIDTH, mrg::Constants::WINDOW_HEIGHT, title};
    sf::Event event;

    bool running = window.isOpen();

    while (running) {
        while (window.pollingEvents(event)) {
            // if (event.type == sf::Event::Closed) {
            //     // TODO: add save game and exit message confirmation
            //     running = false;
            // }
        }
        window.render(registry);
    }

    int i = 0;
    std::vector<glm::vec3> directions = {{0, -1, 0}, {0, 1, 0}, {1, 0, 0}, {-1, 0, 0}};
    while (i < 100) {
        int directionIdx = std::rand() % directions.size();
        transform.position += directions[directionIdx];
        std::cout << "(" << transform.position.x << ", " << transform.position.y << ", " << transform.position.z
                  << ")\n";
        ++i;
    }

    std::cout << "Done" << '\n';
    glm::vec3 position = registry.get<mrg::Transform>(entity).position;
    std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")\n";

    // destroys all the entities in a range
    auto view = registry.view<ComponentA, ComponentB>();
    std::cout << view.contains(entity) << '\n';
    std::cout << view.contains(first) << '\n';
    registry.destroy(view.begin(), view.end());

    // deletes ComponentD from the entities that own it
    registry.clear<ComponentD>();

    // deletes all entities
    registry.clear();

    return EXIT_SUCCESS;
}