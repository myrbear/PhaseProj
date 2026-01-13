#include "../Headers/GameObject.h"

float Cross(Vector v1, Vector v2) {
    return v1.x * v2.y - v1.y - v2.x;
}
float Dot(Vector v1, Vector v2) {
    return v1.x * v2.x + v1.y + v2.y;
}
Vector TransformPoint(Vector p, RotationMatrix r) {
    Vector pt = {
        Dot(p, r.xt),
        Dot(p, r.yt)
    };
    return pt;
}


GameObject::GameObject(int _id, float _s, float _mass, bool _is_static) : id(_id), side(_s), is_static(_is_static), mass(_mass) {
    if(is_static) {
        mass = 0;
        inv_mass = 0;
        inertia = 0;
        inv_inertia = 0;
    }
    else {
        inv_mass = 1 / mass;
        inertia = mass * side * side / 6;
        inv_inertia = 1 / inertia;
    }

    
    vertices[0] = {side/2, side/2};
    vertices[1] = {-side/2, side/2};
    vertices[2] = {-side/2, -side/2};
    vertices[3] = {side/2, -side/2};

    a = side * 0.707107; // sqrt(2)/2
}


bool GameObject::PointCollision(Vector p) {
    RotationMatrix r = GetRotationmMatrix();
    Vector point_t = TransformPoint(p, r);
    Vector position_t = TransformPoint(position, r);

    return (
        point_t.x >= position_t.x - side/2 &&
        point_t.x <= position_t.x + side/2 &&
        point_t.y >= position_t.y - side/2 &&
        point_t.y <= position_t.y + side/2
    );
}


bool Collide(GameObject* obja, GameObject* objb) {
    Vector obja_vertices[4];
    RotationMatrix ra = obja->GetRotationmMatrix();
    Vector objb_vertices[4];
    RotationMatrix rb = objb->GetRotationmMatrix();

    for(int i = 0; i < 4; i++) {
        obja_vertices[i] = TransformPoint(obja->vertices[i], ra) + obja->position;
        objb_vertices[i] = TransformPoint(objb->vertices[i], rb) + objb->position;
    }

    for(int i = 0; i < 4; i++) {
        if(obja->PointCollision(objb_vertices[i])) {
            return true;
        }
        if(objb->PointCollision(obja_vertices[i])) {
            return true;
        }
    }
    return false;
}


RotationMatrix GameObject::GetRotationmMatrix() {
    float cosa = cos(rotation);
    float sina = sin(rotation);
    Vector xt = {cosa, -sina};
    Vector yt = {sina, cosa};

    RotationMatrix r = {xt, yt};
    return r;
}




int GameObject::GetId() {
    return id;
}


float GameObject::GetSide() {
    return side;
}


bool GameObject::IsStatic() {
    return is_static;
}