#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "raylib.h" 
#include "PhaseEngine.h"

namespace Sidebar {
    // Initialize sidebar UI resources
    void Init();
    

    // Update sidebar (handles input, drag/drop and object creation)
    void Update(PhaseEngine* engine);

    // Draw the sidebar UI
    void Draw();

    // Shutdown / cleanup
    void Shutdown();
}

#endif // SIDEBAR_H