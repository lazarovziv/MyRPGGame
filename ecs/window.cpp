#include "window.hpp"

namespace rg {
    Window::Window(const uint32_t width, const uint32_t height, const std::string& title) {
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode{width, height}, title);
        window->setVerticalSyncEnabled(false);
        // window->setFramerateLimit(Constants::FPS);
        window->setFramerateLimit(0);
        cameraView = std::make_unique<sf::View>(sf::Vector2f{0, 0}, sf::Vector2f(width, height));

        cameraView->setCenter(sf::Vector2f(rg::Constants::WINDOW_WIDTH / 2, rg::Constants::WINDOW_HEIGHT));
        window->setView(*cameraView);
    }

    void Window::render(const glm::vec3& cameraPosition, const entt::registry& registry) {
        renderer.render(window, cameraView, cameraPosition, registry);
    }
} // rg
