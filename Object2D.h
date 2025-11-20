#pragma once
#ifndef OBJECT2D_H
#define OBJECT2D_H

#include <SFML/Graphics.hpp>

class Object2D {
public:
    sf::Vector2f position;
    float radius;
    sf::Color color;

    Object2D(sf::Vector2f pos = {0.f, 0.f}, float r = 0.f, sf::Color c = sf::Color::White);
};

#endif
