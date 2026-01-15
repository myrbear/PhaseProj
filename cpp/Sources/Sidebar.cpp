#include "../Headers/Sidebar.h"
#include <vector>
#include <string>

using namespace std;

namespace Sidebar {

    static Rectangle sidebarRect;
    static const int sidebarWidth = 220;

    struct PaletteItem {
        Rectangle rect;
        float side;
        bool is_static;
        Color color;
        string label;
    };

    static vector<PaletteItem> items;

    // Drag state
    static bool dragging = false;
    static float dragSide = 32;
    static bool dragIsStatic = false;
    static Color dragColor = GRAY;

    void Init() {
        sidebarRect = { 0.0f, 0.0f, (float)sidebarWidth, (float)GetScreenHeight() };

        items.clear();
        float pad = 20;
        float y = 40;

        // Dynamic small
        PaletteItem p1;
        p1.side = 32;
        p1.is_static = false;
        p1.color = RED;
        p1.label = "Dynamic 32";
        p1.rect = { 20.0f, y, p1.side, p1.side };
        items.push_back(p1);
        y += p1.side + pad;

        // Dynamic large
        PaletteItem p2;
        p2.side = 64;
        p2.is_static = false;
        p2.color = ORANGE;
        p2.label = "Dynamic 64";
        p2.rect = { 20.0f, y, p2.side, p2.side };
        items.push_back(p2);
        y += p2.side + pad;

        // Static
        PaletteItem p3;
        p3.side = 48;
        p3.is_static = true;
        p3.color = BLUE;
        p3.label = "Static 48";
        p3.rect = { 20.0f, y, p3.side, p3.side };
        items.push_back(p3);
    }

    void Update(PhaseEngine* engine) {
        Vector2 mpos = GetMousePosition();

        // Start drag when clicking on a palette item
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !dragging) {
            for (auto &it : items) {
                if (CheckCollisionPointRec(mpos, it.rect)) {
                    dragging = true;
                    dragSide = it.side;
                    dragIsStatic = it.is_static;
                    dragColor = it.color;
                    break;
                }
            }
        }

        // On release, if dragging and released in main area (to the right of sidebar), create object
        if (dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (mpos.x > sidebarWidth) {
                int id;
                if (dragIsStatic) {
                    id = engine->CreateStaticObject(dragSide, 0);
                }
                else {
                    id = engine->CreateObject(dragSide, 1.0f, 0);
                }

                if (id != -1) {
                    // Set position so the object is centered at drop point
                    engine->SetPosition(id, mpos.x, mpos.y);
                }
            }

            dragging = false;
        }
    }

    void Draw() {
        // Sidebar background
        DrawRectangleRec(sidebarRect, DARKGRAY);

        // Title
        DrawText("Sidebar", 20, 8, 20, WHITE);

        // Draw palette items
        for (auto &it : items) {
            DrawRectangleRec(it.rect, it.color);
            DrawRectangleLines((int)it.rect.x, (int)it.rect.y, (int)it.rect.width, (int)it.rect.height, BLACK);
            DrawText(it.label.c_str(), (int)(it.rect.x + it.rect.width + 8), (int)it.rect.y + 8, 12, WHITE);
        }

        // Draw drag preview
        if (dragging) {
            Vector2 mpos = GetMousePosition();
            Rectangle preview = { mpos.x - dragSide/2.0f, mpos.y - dragSide/2.0f, dragSide, dragSide };
            DrawRectangleRec(preview, Fade(dragColor, 0.7f));
            DrawRectangleLines((int)preview.x, (int)preview.y, (int)preview.width, (int)preview.height, BLACK);
        }
    }

    void Shutdown() {
        items.clear();
        dragging = false;
    }
} // namespace Sidebar