#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "raylib.h"
#include "PhaseEngine.h"

namespace ControlPanel {
    // Initialize panel resources
    void Init();

    // Update panel; will call engine setters as sliders move
    void Update(PhaseEngine* engine);

    // Draw the control panel (on the right side)
    void Draw();

    void Shutdown();
}

#endif // CONTROLPANEL_H
