#ifndef COLLIDER_H
#define COLLIDER_H

#include "vector.h"
#include "quaternion.h"
#include "ll.h"

#define VERT_COUNT 10

struct Collider {

	Vector _pos;
        Quaternion _rot;
        Vector _verts[VERT_COUNT];
        Vector _scale;
};

void init_col(Collider* col);
Vector** gen_faces(Vector* v_arr, Vector dir);
Vector get_farthest(Collider col0, Vector dir);
Vector support(Collider* col0, Collider* col1, Vector dir);
Vector intersect(Collider* col0, Collider* col1);

#endif // COLLIDER_H

