#ifndef COLLIDER_H
#define COLLIDER_H

#include "vector.h"
#include "quaternion.h"
#include "ll.h"

#define VERT_COUNT 10

struct Collider {

	Vector _pos;
        Quaternion _rot;
        Node* _verts;
        Vector _scale;
};

void p_col(Collider col);
void init_col(Collider* col, float x0, float x1, float x2, float y0, float y1, float y2, float xp, float yp);
    Vector** gen_faces(Node* polytope, int len);
Vector get_farthest(Collider col0, Vector dir);
Vector support(Collider col0, Collider col1, Vector dir);
int intersect(Collider* col0, Collider* col1);

#endif // COLLIDER_H

