#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <iostream>
#include <cmath>

#define PI 3.14159265

#define VANG1 0.7854
#define VANG2 2.3562
#define VANG3 3.9270
#define VANG4 5.4978


// Vector
struct Vector {
    float x;
    float y;
    Vector operator+(Vector other) {
        Vector sum = {x + other.x, y + other.y};
        return sum;
    }
    Vector operator-() {
        Vector negative = {-x,-y};
        return negative;
    }
    Vector operator-(Vector other) {
        Vector difference = {x - other.x, y - other.y};
        return difference;
    }
    Vector operator/(float c) {
        Vector quotient = {x/c, y/c};
        return quotient;
    }
    Vector operator*(float c) {
        Vector product = {x*c, y*c};
        return product;
    }
};

float Cross(Vector v1, Vector v2);
float Dot(Vector v1, Vector v2);
// bool ProjectOntoSegment(Vector p, Vector dir, Vector a, Vector b, Vector* out);

struct RotationMatrix {
    Vector xt;
    Vector yt;

    Vector GetXNormal() {
        Vector xn = {xt.x, yt.x};
        return xn;
    }
    Vector GetYNormal() {
        Vector yn = {xt.y, yt.y};
        return yn;
    }
};

Vector TransformPoint(Vector p, RotationMatrix r);

class GameObject
{
    public:
        GameObject(int id, float _s, float _mass, bool _is_static, int _color);

        int GetId();
        float GetSide();
        bool IsStatic();
        bool PointCollision(Vector p);
        RotationMatrix GetRotationMatrix();
        RotationMatrix GetReverseRotationMatrix();


        Vector position = {0, 0};
        Vector velocity = {0, 0};
        float rotation = 0;
        float angular_velocity = 0;

        Vector vertices[4];

        float mass;
        float inv_mass;
        float inertia;
        float inv_inertia;

        int color;
        
    private:
        float side;
        float a;
        int id;
        bool is_static;
};

struct CollisionInfo {
    Vector contact_point;
    Vector contact_normal;
    float penetration_depth;
};

// Static functions
bool Collide(GameObject* obja, GameObject* objb, CollisionInfo* info);
bool AxisSeparation(GameObject* obja, GameObject* objb, CollisionInfo *info);

float min_clamp0(float a, float b);
float min_clamp0(float a, float b, float c, float d);

#endif // GAME_OBJECT
