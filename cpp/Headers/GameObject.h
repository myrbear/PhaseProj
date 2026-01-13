#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <iostream>
#include <cmath>

#define VANG1 0.7854
#define VANG2 2.3562
#define VANG3 3.9270
#define VANG4 5.4978


// Vector
struct Vector {
    float x;
    float y;
    Vector operator+(Vector &other) {
        Vector sum = {
            x + other.x,
            y + other.y
        };
        return sum;
    }
};

float Cross(Vector v1, Vector v2);
float Dot(Vector v1, Vector v2);

struct RotationMatrix {
    Vector xt;
    Vector yt;
};

Vector TransformPoint(Vector p, RotationMatrix r);

class GameObject
{
    public:
        GameObject(int id, float _s, float _mass, bool _is_static);

        int GetId();
        float GetSide();
        bool IsStatic();
        bool PointCollision(Vector p);
        RotationMatrix GetRotationmMatrix();

        Vector position = {0, 0};
        Vector velocity = {0, 0};
        float rotation = 0;
        float angular_velocity = 0;

        Vector vertices[4];
        
    private:
        float side;
        float a;
        int id;
        bool is_static;
        float mass;
        float inv_mass;
        float inertia;
        float inv_inertia;

};

bool Collide(GameObject* obja, GameObject* objb);

#endif // GAME_OBJECT
