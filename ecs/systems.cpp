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

        std::cout << "(" << sprite.getPosition().x << ", " << sprite.getPosition().y << ")\n";

        // cameraView->setCenter(sprite.getPosition());
        // window->setView(*cameraView);

        // rendering at the relevant location
        window->draw(sprite);
    }

    window->display();
}
} // namespace mrg