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

    engine.SetPosition(obj1, 90, 100);
    engine.SetPosition(obj2, 200, 100);
    engine.SetPosition(obj3, 310, 100);

    engine.SetRotation(obj1, 30);
    engine.SetRotation(obj2, 45);
    engine.SetRotation(obj3, 60);

    int obj4 = engine.CreateStaticObject(32);
    int obj5 = engine.CreateStaticObject(32);
    int obj6 = engine.CreateStaticObject(32);

    engine.SetPosition(obj4, 100, 600);
    engine.SetPosition(obj5, 200, 600);
    engine.SetPosition(obj6, 300, 600);

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

        Vector2 mouse_pos = GetMousePosition();

        for(auto it = engine.BeginObjIt(); it != engine.EndObjIt(); it++)
        {
            GameObject* obj = *it;

            float x = obj->position.x;
            float y = obj->position.y;
            float side = obj->GetSide();
            Vector2 origin = {side/2, side/2};
            Rectangle rect = {x, y, side, side};

            DrawRectanglePro(rect, origin, obj->rotation, RED);

            RotationMatrix r = obj->GetRotationMatrix();

            // for(int i = 0; i < 4; i++) {
            //     Vector vertex = TransformPoint(obj->vertices[i], r) + obj->position;
            //     DrawCircle(vertex.x, vertex.y, 2, BLUE);
            // }
        }

        // for(auto it = engine.BeginObjIt(); it != engine.EndObjIt(); it++)
        // {
        //     GameObject* obj = *it;
        //     DrawCircle(obj->pos.x, obj->pos.y, 2, GREEN);
        // }

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