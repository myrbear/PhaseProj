#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Object2D.h"

// Simple editor-style display for 2D objects.
// Right-hand side panel lets you choose shape type and add new objects.
class Display {
public:
    Display();

    // Handle window + user input
    void pollEvents(std::vector<Object2D>& objs);

    // Frame lifecycle
    void clear();
    void drawObjects(const std::vector<Object2D>& objs);
    void drawUI();
    void display();

    bool isOpen() const;
    ShapeType getSelectedShapeType() const { return selectedShapeType; }

private:
    sf::RenderWindow window;
    ShapeType        selectedShapeType;
    sf::Vector2f     lastMousePos;

    // Rendering helpers
    void drawShape(const Object2D& obj);
    void drawSidePanel();

    // Hit testing for the side panel
    bool isInSidePanel(sf::Vector2f pos) const;
};
