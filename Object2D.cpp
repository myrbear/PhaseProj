#include "Object2D.h"
#include <cmath>

Object2D::Object2D(sf::Vector2f pos, float r, sf::Color c, ShapeType type)
    : position(pos)
    , radius(r)
    , color(c)
    , shapeType(type)
    , isDragging(false)
    , dragOffset(0.f, 0.f)
{
}

bool Object2D::containsPoint(sf::Vector2f point) const {
    // Simple circular hit test centered on the visual center of the object
    sf::Vector2f center = position + sf::Vector2f(radius, radius);
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    float distSq = dx * dx + dy * dy;
    return distSq <= (radius * radius);
}

void Object2D::startDrag(sf::Vector2f mousePos) {
    isDragging = true;

    // Offset between mouse and object center so the shape doesn't jump
    sf::Vector2f center = position + sf::Vector2f(radius, radius);
    dragOffset = mousePos - center;
}

void Object2D::stopDrag() {
    isDragging = false;
}

void Object2D::updatePosition(sf::Vector2f mousePos) {
    if (isDragging) {
        // Maintain the offset while dragging
        sf::Vector2f newCenter = mousePos - dragOffset;
        position = newCenter - sf::Vector2f(radius, radius);
    }
}

sf::FloatRect Object2D::getBounds() const {
    return sf::FloatRect(position, sf::Vector2f(radius * 2.f, radius * 2.f));
}
