#include "../Headers/main.h"
#include <iostream>

using namespace std;

int main()
{
    
    InitWindow(screen_width, screen_height, "Game");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    PhaseEngine engine;

    int obj1 = engine.CreateObject(32, 1);
    int obj2 = engine.CreateObject(32, 1);
    int obj3 = engine.CreateObject(32, 1);

    engine.SetPosition(obj1, 100, 100);
    engine.SetPosition(obj2, 200, 100);
    engine.SetPosition(obj3, 300, 100);

    int obj4 = engine.CreateStaticObject(32);
    int obj5 = engine.CreateStaticObject(32);
    int obj6 = engine.CreateStaticObject(32);

    engine.SetPosition(obj4, 100, 300);
    engine.SetPosition(obj5, 200, 300);
    engine.SetPosition(obj6, 300, 300);

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

        for(auto it = engine.BeginObjIt(); it != engine.EndObjIt(); it++)
        {
            GameObject* obj = *it;

            DrawRectangle(obj->position.x, obj->position.y, obj->GetSide(), obj->GetSide(), RED);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    engine.Stop();

    cout << "Program Successfully Executed" << endl;

    return 0;
}