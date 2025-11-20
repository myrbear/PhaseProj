#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Object2D.h"

class Display {
public:
    Display();

    void pollEvents();
    void clear();
    void drawObjects(const std::vector<Object2D>& objs);
    void display();
    bool isOpen() const;

private:
    sf::RenderWindow window;
};
