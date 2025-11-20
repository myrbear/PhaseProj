#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Display.h"
#include "Object2D.h"

int main() {
    Display display;

    std::vector<Object2D> objects;
    objects.emplace_back(sf::Vector2f{200, 200}, 30, sf::Color::Green);
    objects.emplace_back(sf::Vector2f{400, 300}, 50, sf::Color::Blue);
    objects.emplace_back(sf::Vector2f{600, 400}, 20, sf::Color::Red);

    while (display.isOpen()) {
        display.pollEvents();
        display.clear();
        display.drawObjects(objects);
        display.display();
    }

    return 0;
}
