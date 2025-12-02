#include <vector>
#include "SFML-3.0.2/include/SFML/Graphics.hpp"
#include "Display.h"
#include "Object2D.h"

int main() {
    Display display;

    std::vector<Object2D> objects;
    objects.emplace_back(sf::Vector2f{200.f, 200.f}, 30.f, sf::Color::Green,  ShapeType::Circle);
    objects.emplace_back(sf::Vector2f{400.f, 300.f}, 50.f, sf::Color::Blue,   ShapeType::Square);
    objects.emplace_back(sf::Vector2f{600.f, 400.f}, 20.f, sf::Color::Red,    ShapeType::Triangle);

    while (display.isOpen()) {
        display.pollEvents(objects);
        display.clear();
        display.drawObjects(objects);
        display.drawUI();
        display.display();
    }

    return 0;
}
