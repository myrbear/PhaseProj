#include "Display.h"
#include <cmath>

namespace {
    constexpr unsigned WINDOW_WIDTH  = 1000;
    constexpr unsigned WINDOW_HEIGHT = 600;
    constexpr float    PANEL_X       = 800.f;
    constexpr float    PANEL_WIDTH   = 200.f;

    // Button vertical ranges inside the side panel
    constexpr float BTN_HEIGHT   = 40.f;
    constexpr float BTN_GAP      = 10.f;

    constexpr float BTN_CIRCLE_Y  = 100.f;
    constexpr float BTN_SQUARE_Y  = BTN_CIRCLE_Y + BTN_HEIGHT + BTN_GAP;
    constexpr float BTN_TRIANGLE_Y= BTN_SQUARE_Y + BTN_HEIGHT + BTN_GAP;
    constexpr float BTN_ADD_Y     = 300.f;
}

Display::Display()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "PHASE Display")
    , selectedShapeType(ShapeType::Circle)
    , lastMousePos(0.f, 0.f)
{
    window.setFramerateLimit(60);
}

void Display::pollEvents(std::vector<Object2D>& objs) {
    while (auto event = window.pollEvent()) {
        // Close event
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        // Mouse button pressed
        if (auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f mousePos(
                static_cast<float>(mousePress->position.x),
                static_cast<float>(mousePress->position.y)
            );

            // Clicked inside the side panel: treat as UI click
            if (isInSidePanel(mousePos)) {
                const float y = mousePos.y;

                // Shape selection buttons
                if (y >= BTN_CIRCLE_Y && y <= BTN_CIRCLE_Y + BTN_HEIGHT) {
                    selectedShapeType = ShapeType::Circle;
                } else if (y >= BTN_SQUARE_Y && y <= BTN_SQUARE_Y + BTN_HEIGHT) {
                    selectedShapeType = ShapeType::Square;
                } else if (y >= BTN_TRIANGLE_Y && y <= BTN_TRIANGLE_Y + BTN_HEIGHT) {
                    selectedShapeType = ShapeType::Triangle;
                }
                // Add-object button
                else if (y >= BTN_ADD_Y && y <= BTN_ADD_Y + BTN_HEIGHT) {
                    sf::Vector2f spawnPos(200.f, 200.f); // simple default
                    float r = 30.f;

                    sf::Color color = sf::Color::Red;
                    if (selectedShapeType == ShapeType::Square)   color = sf::Color::Blue;
                    if (selectedShapeType == ShapeType::Triangle) color = sf::Color::Green;

                    objs.emplace_back(spawnPos, r, color, selectedShapeType);
                }
            }
            // Clicked in the main canvas: select object to drag
            else {
                // Iterate in reverse so top-most / last created gets priority
                for (auto it = objs.rbegin(); it != objs.rend(); ++it) {
                    if (it->containsPoint(mousePos)) {
                        it->startDrag(mousePos);
                        break;
                    }
                }
            }
        }

        // Mouse button released: stop dragging any object
        if (event->is<sf::Event::MouseButtonReleased>()) {
            for (auto& obj : objs) {
                if (obj.isDragging) {
                    obj.stopDrag();
                }
            }
        }

        // Mouse moved: update positions of any dragging objects
        if (auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePos(
                static_cast<float>(mouseMove->position.x),
                static_cast<float>(mouseMove->position.y)
            );
            lastMousePos = mousePos;

            for (auto& obj : objs) {
                if (obj.isDragging) {
                    obj.updatePosition(mousePos);
                }
            }
        }
    }
}

void Display::clear() {
    // Light gray background
    window.clear(sf::Color(230, 230, 230));
}

void Display::drawShape(const Object2D& obj) {
    switch (obj.shapeType) {
    case ShapeType::Circle: {
        sf::CircleShape shape(obj.radius);
        shape.setFillColor(obj.color);
        shape.setPosition(obj.position);
        window.draw(shape);
        break;
    }
    case ShapeType::Square: {
        sf::RectangleShape shape(sf::Vector2f(obj.radius * 2.f, obj.radius * 2.f));
        shape.setFillColor(obj.color);
        shape.setPosition(obj.position);
        window.draw(shape);
        break;
    }
    case ShapeType::Triangle: {
        sf::ConvexShape triangle(3);
        triangle.setPoint(0, sf::Vector2f(obj.radius, 0.f));
        triangle.setPoint(1, sf::Vector2f(obj.radius * 2.f, obj.radius * 2.f));
        triangle.setPoint(2, sf::Vector2f(0.f, obj.radius * 2.f));
        triangle.setFillColor(obj.color);
        triangle.setPosition(obj.position);
        window.draw(triangle);
        break;
    }
    }
}

void Display::drawObjects(const std::vector<Object2D>& objs) {
    for (const auto& obj : objs) {
        drawShape(obj);
    }
}

bool Display::isInSidePanel(sf::Vector2f pos) const {
    return pos.x >= PANEL_X && pos.x <= PANEL_X + PANEL_WIDTH;
}

void Display::drawSidePanel() {
    // Background
    sf::RectangleShape panel(sf::Vector2f(PANEL_WIDTH, static_cast<float>(WINDOW_HEIGHT)));
    panel.setPosition(sf::Vector2f(PANEL_X, 0.f));
    panel.setFillColor(sf::Color(200, 200, 200));
    window.draw(panel);

    // Helper to draw a button
    auto drawButton = [&](float y, bool selected) {
        sf::RectangleShape btn(sf::Vector2f(PANEL_WIDTH - 20.f, BTN_HEIGHT));
        btn.setPosition(sf::Vector2f(PANEL_X + 10.f, y));
        btn.setFillColor(selected ? sf::Color(160, 160, 220) : sf::Color(180, 180, 180));
        btn.setOutlineThickness(1.f);
        btn.setOutlineColor(sf::Color::Black);
        window.draw(btn);
    };

    // Shape selection buttons
    drawButton(BTN_CIRCLE_Y,   selectedShapeType == ShapeType::Circle);
    drawButton(BTN_SQUARE_Y,   selectedShapeType == ShapeType::Square);
    drawButton(BTN_TRIANGLE_Y, selectedShapeType == ShapeType::Triangle);

    // Add-object button
    drawButton(BTN_ADD_Y, false);

    // Border line between canvas and panel
    sf::RectangleShape border(sf::Vector2f(2.f, static_cast<float>(WINDOW_HEIGHT)));
    border.setPosition(sf::Vector2f(PANEL_X, 0.f));
    border.setFillColor(sf::Color::Black);
    window.draw(border);
}

void Display::drawUI() {
    drawSidePanel();
    // Text labels could be added later using sf::Text and a font
}

void Display::display() {
    window.display();
}

bool Display::isOpen() const {
    return window.isOpen();
}
