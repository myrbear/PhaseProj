#include "../Headers/ControlPanel.h"
#include "raylib.h"

namespace ControlPanel {

    static Rectangle panelRect;
    static const int panelWidth = 220;

    static float gravityNorm = 0.2f; // normalized 0..1
    static float frictionNorm = 0.1f;
    static bool draggingGravity = false;
    static bool draggingFriction = false;

    void Init(PhaseEngine* engine) {
        panelRect = { (float)(GetScreenWidth() - panelWidth), 0.0f, (float)panelWidth, (float)GetScreenHeight() };
        engine->SetGravity(gravityNorm * 20.0f);
    }

    static float clampf(float v, float a, float b) { return (v < a) ? a : (v > b) ? b : v; }

    void Update(PhaseEngine* engine) {
        Vector2 mpos = GetMousePosition();
        Vector2 local = { mpos.x - panelRect.x, mpos.y - panelRect.y };

        const float trackX = panelRect.x + 20.0f;
        const float trackW = panelRect.width - 40.0f;
        const float gravityY = panelRect.y + 120.0f;
        const float frictionY = panelRect.y + 200.0f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // gravity handle vicinity
            float gx = trackX + gravityNorm * trackW;
            if (mpos.x >= gx - 10 && mpos.x <= gx + 10 && mpos.y >= gravityY - 12 && mpos.y <= gravityY + 12) {
                draggingGravity = true;
            }
            float fx = trackX + frictionNorm * trackW;
            if (mpos.x >= fx - 10 && mpos.x <= fx + 10 && mpos.y >= frictionY - 12 && mpos.y <= frictionY + 12) {
                draggingFriction = true;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            draggingGravity = false;
            draggingFriction = false;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (draggingGravity) {
                float nx = (local.x - 20.0f) / trackW;
                gravityNorm = clampf(nx, 0.0f, 1.0f);
                // Map normalized gravity to engine units: 0..20
                engine->SetGravity(gravityNorm * 20.0f);
            }
            if (draggingFriction) {
                float nx = (local.x - 20.0f) / trackW;
                frictionNorm = clampf(nx, 0.0f, 1.0f);
                // Map to damping value 0..5
                engine->SetFriction(frictionNorm * 5.0f);
            }
        }
    }

    void Draw() {
        // panel
        Rectangle panel = panelRect;
        DrawRectangleRec(panel, LIGHTGRAY);
        DrawRectangleLines((int)panel.x, (int)panel.y, (int)panel.width, (int)panel.height, BLACK);

        // Labels
        DrawText("Controls", (int)panel.x + 10, (int)panel.y + 8, 20, BLACK);
        DrawText("Gravity", (int)panel.x + 10, (int)panel.y + 90, 14, DARKGRAY);
        DrawText("Friction", (int)panel.x + 10, (int)panel.y + 170, 14, DARKGRAY);

        const float trackX = panel.x + 20.0f;
        const float trackW = panel.width - 40.0f;
        const float gravityY = panel.y + 120.0f;
        const float frictionY = panel.y + 200.0f;

        // gravity track
        DrawRectangle((int)trackX, (int)(gravityY - 3), (int)trackW, 6, GRAY);
        float gx = trackX + gravityNorm * trackW;
        DrawCircle((int)gx, (int)gravityY, 8, BLUE);
        // friction track
        DrawRectangle((int)trackX, (int)(frictionY - 3), (int)trackW, 6, GRAY);
        float fx = trackX + frictionNorm * trackW;
        DrawCircle((int)fx, (int)frictionY, 8, GREEN);

        // numeric values
        char buf[64];
        sprintf(buf, "%.0f", gravityNorm * 20.0f);
        DrawText(buf, (int)panel.x + 10, (int)panel.y + 60, 14, BLACK);
        sprintf(buf, "%.2f", frictionNorm * 5.0f);
        DrawText(buf, (int)panel.x + 10, (int)panel.y + 140, 14, BLACK);
    }

    void Shutdown() {
        // nothing to do
    }

} // namespace ControlPanel
