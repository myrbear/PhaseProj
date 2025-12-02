#pragma once
#ifndef OBJECT2D_H
#define OBJECT2D_H

#include <SFML/Graphics.hpp>

enum class ShapeType {
    Circle,
    Square,
    Triangle
};

class Object2D {
public:
    sf::Vector2f position;     // Top-left corner of the object
    float        radius;       // Radius for circle, half-size for square/triangle
    sf::Color    color;
    ShapeType    shapeType;

    // Dragging state
    bool         isDragging;
    sf::Vector2f dragOffset;

    Object2D(sf::Vector2f pos = {0.f, 0.f},
             float        r   = 0.f,
             sf::Color    c   = sf::Color::White,
             ShapeType    type = ShapeType::Circle);

    // Hit-test against the *visual bounds* (simple circle-based test)
    bool containsPoint(sf::Vector2f point) const;

    // Dragging helpers
    void startDrag(sf::Vector2f mousePos);
    void stopDrag();
    void updatePosition(sf::Vector2f mousePos);

    // Axis-aligned bounding box
    sf::FloatRect getBounds() const;
};

#endif
