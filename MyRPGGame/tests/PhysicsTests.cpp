#include "../include/Collision.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace physics;

void test() {
    RenderWindow window(VideoMode(800, 600), "Physics Test");

    sf::Clock clock;
    // TODO: tweak multiplier value (12 seems to be close, maybe fps/10)
    real dt, multiplier = 12.f;

    std::vector<RigidBody*> bodies = {};
    std::vector<Shape*> shapes = {};
    Vector mouse = Vector::ZERO;

    int numBodies = 0;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();
        dt *= multiplier;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        mouse.x = Mouse::getPosition().x;
        mouse.y = Mouse::getPosition().y;

        if (Mouse::isButtonPressed(Mouse::Button::Left)) {
            // adding a body
            int random = rand() % 2;
            if (random == 0) {
                bodies.push_back(new physics::Circle(mouse.x, mouse.y, 0, 25));
                CircleShape shape(25);
                shape.setPosition(bodies[numBodies]->getPosition().x, bodies[numBodies]->getPosition().y);
                shape.setFillColor(Color::Blue);
            }
            else {
                bodies.push_back(new physics::Polygon(Mouse::getPosition().x, Mouse::getPosition().y, 0, {
                        Vector{mouse.x - 32, mouse.y - 32}, // top left
                        Vector{mouse.x + 32, mouse.y - 32}, // top right
                        Vector{mouse.x - 32, mouse.y + 32}, // bottom left
                        Vector{mouse.x + 32, mouse.y + 32}, // bottom right
                }, 1));
                RectangleShape shape;
                shape.setPosition(((Polygon*)bodies[numBodies])->getVertices()->at(0).x, ((Polygon*)bodies[numBodies])->getVertices()->at(0).y);
                shape.setFillColor(Color::Red);
            }
            numBodies++;
        }

        for (auto first : bodies) {
            for (auto second : bodies) {
                if (first == second) continue;
                resolveCollisions(first, second, dt);
            }
        }

        window.clear();

        for (int i = 0; i < bodies.size(); i++) {
            window.draw(*shapes[i]);
        }

        window.display();
    }
}


