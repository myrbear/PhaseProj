#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Headers/vector.h"
#include "../Headers/quaternion.h"

#define VERT_COUNT 10

class Collider {
    public:
        Collider();

        Vector _pos;
        Quaternion _rot;
        Vector _verts[VERT_COUNT];
        Vector _scale;
    private:

};

#endif // COLLIDER_H