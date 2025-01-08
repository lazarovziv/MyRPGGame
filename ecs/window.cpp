#include "window.hpp"

namespace mrg {

Window::Window(const uint32_t width, const uint32_t height, const std::string &title) {
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode{width, height}, title);
    window->setVerticalSyncEnabled(false);
    // window->setFramerateLimit(Constants::FPS);
    window->setFramerateLimit(0);
    cameraView = std::make_unique<sf::View>(sf::Vector2f{0, 0}, sf::Vector2f(width, height));

    cameraView->setCenter(sf::Vector2f(0, 0));
    window->setView(*cameraView);
}

void Window::render(entt::registry &registry) {
    renderer.render(window, cameraView, registry);
}

} // namespace mrg