#include "systems.hpp"

#include "glm/vec3.hpp"

namespace mrg {

void Renderer::render(std::unique_ptr<sf::RenderWindow> &window, std::unique_ptr<sf::View> &cameraView,
                      const entt::registry &registry) {
    auto view = registry.view<Renderable, Transform>();

    // clearing the previous frame
    window->clear();
    // draw background

    // traversing all entities
    for (auto &entity : view) {
        auto renderable = view.get<Renderable>(entity);
        sf::Sprite &sprite = renderable.sprite;
        auto transform = view.get<Transform>(entity);
        glm::vec3 &position = transform.position;

        renderable.setPosition(position);

        // rendering at the relevant location
        window->draw(sprite);
    }

    window->display();
}

// important: if registry will be const, the view will be const and the modifications of the components' data won't work
void Movement::move(entt::registry &registry, const real dt) {
    auto view = registry.view<Transform, Moveable>();

    view.each([&](auto &transform, auto &moveable) {
        std::cout << transform.position.x << ',' << transform.position.y << '\n';
        transform.position += moveable.direction * (1000 * dt);
        std::cout << transform.position.x << ',' << transform.position.y << '\n';
    });

    // for (auto &entity : view) {
    //     auto &transform = view.get<Transform>(entity);
    //     // assuming the direction vector is already normalized
    //     auto &moveable = view.get<Moveable>(entity);

    //     std::cout << transform.position.x << ',' << transform.position.y << '\n';
    //     transform.position += moveable.direction * (1000 * dt);
    //     std::cout << transform.position.x << ',' << transform.position.y << '\n';
    // }
}

} // namespace mrg