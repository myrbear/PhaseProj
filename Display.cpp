#include "Display.h"

void Display::pollEvents() {

    while (auto event = window.pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

Display::Display()
    : window(sf::VideoMode({800, 600}), "PHASE Display") 
{
}

void Display::clear() {
    window.clear(sf::Color::Black);
}

void Display::drawObjects(const std::vector<Object2D>& objs) {

    for (const auto& o : objs) {
        sf::CircleShape shape(o.radius);
        shape.setFillColor(o.color);
        shape.setPosition(o.position);
        window.draw(shape);
    }
}

void Display::display() {
    window.display();
}

bool Display::isOpen() const {
    return window.isOpen();
}
