#include "../src/common/include/Constants.h"
#include <SFML/Graphics.hpp>

namespace PhysicsSimulation {
    void simulate() {
        int SCREEN_WIDTH = 800;
        int SCREEN_HEIGHT = 600;
        sf::VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
        std::string s = "Physics Simulation";
        sf::RenderWindow = sf::RenderWindow(videoMode, s);

        window->setVerticalSyncEnabled(false);
    //    window->setFramerateLimit(Constants::FPS);
        window->setFramerateLimit(0);

        sf::Rect rect(sf::Vector2f(100, 100));

        while (window.isOpen()) {
            window.clear();

            window.draw(rect);

            window.display();
        }

    }
}