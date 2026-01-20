#include "../Headers/main.h"

using namespace std;

int main()
{
    
    InitWindow(screen_width, screen_height, "PHASE Editor");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    PhaseEngine engine;

    // Initialize sidebar
    Sidebar::Init();
    ControlPanel::Init();

    engine.Run();

    // Main window loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // Update sidebar input (handles drag/drop and object creation)
        Sidebar::Update(&engine);
        ControlPanel::Update(&engine);

        for(auto it = engine.BeginObjIt(); it != engine.EndObjIt(); it++)
        {
            GameObject* obj = *it;

            float x = obj->position.x;
            float y = obj->position.y;
            float side = obj->GetSide();
            Vector2 origin = {side/2, side/2};
            Rectangle rect = {x, y, side, side};

            Color color;
            switch (obj->color) {
                case 0: color = RED; 
                    break;
                case 1: color = ORANGE; 
                    break;
                case 2: color = BLUE; 
                    break;
                default: color = WHITE; 
                    break;
            };

            DrawRectanglePro(rect, origin, obj->rotation, color);
        }

        // DRAG LOGIC HERE

        // DRAG LOGIC HERE

        // Draw sidebar on top of scene
        Sidebar::Draw();
        ControlPanel::Draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    engine.Stop();

    Sidebar::Shutdown();
    ControlPanel::Shutdown();

    cout << "Program Successfully Executed" << endl;

    return 0;
}