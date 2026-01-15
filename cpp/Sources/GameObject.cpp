#include "../Headers/GameObject.h"

float Cross(Vector v1, Vector v2) {
    return v1.x * v2.y - v1.y * v2.x;
}
float Dot(Vector v1, Vector v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
Vector TransformPoint(Vector p, RotationMatrix r) {
    Vector pt = {
        Dot(p, r.xt),
        Dot(p, r.yt)
    };
    return pt;
}


GameObject::GameObject(int _id, float _s, float _mass, bool _is_static, int _color) : id(_id), side(_s), is_static(_is_static), mass(_mass), color(_color) {
    if(is_static) {
        mass = 0;
        inv_mass = 0;
        inertia = 0;
        inv_inertia = 0;
    }
    else {
        inv_mass = 1 / mass;
        inertia = mass * side * side / 6 / 32;
        inv_inertia = 1 / inertia;
    }

    
    vertices[0] = {side/2, side/2};
    vertices[1] = {-side/2, side/2};
    vertices[2] = {-side/2, -side/2};
    vertices[3] = {side/2, -side/2};

    a = side * 0.707107; // sqrt(2)/2
}


bool GameObject::PointCollision(Vector p) {
    RotationMatrix r = GetReverseRotationMatrix();
    Vector point_t = TransformPoint(p, r);
    Vector position_t = TransformPoint(position, r);
    
    return (
        point_t.x >= position_t.x - side/2 &&
        point_t.x <= position_t.x + side/2 &&
        point_t.y >= position_t.y - side/2 &&
        point_t.y <= position_t.y + side/2
    );
}


bool Collide(GameObject* obja, GameObject* objb, CollisionInfo* info) {
    Vector obja_vertices[4];
    RotationMatrix ra = obja->GetRotationMatrix();
    Vector objb_vertices[4];
    RotationMatrix rb = objb->GetRotationMatrix();

    for(int i = 0; i < 4; i++) {
        obja_vertices[i] = TransformPoint(obja->vertices[i], ra) + obja->position;
        objb_vertices[i] = TransformPoint(objb->vertices[i], rb) + objb->position;
    }

    bool collision = false;

    for(int i = 0; i < 4; i++) {
        if(obja->PointCollision(objb_vertices[i])) {
            collision = true;
            break;
        }
        if(objb->PointCollision(obja_vertices[i])) {
            collision = true;
            break;
        }
    }
    
    if(!collision) return false;

    // Collision (Separating Axis Theorem)
    CollisionInfo info_a;
    CollisionInfo info_b;
    AxisSeparation(obja, objb, &info_a);
    AxisSeparation(objb, obja, &info_b);

    if(info_a.penetration_depth < info_b.penetration_depth) {
        // Use A's normal
        *info = info_a;
    }
    else {
        // Use B's normal (and flip it, contact normal is A->B)
        info_b.contact_normal = -info_b.contact_normal;
        *info = info_b;
    }

    return true;
}

// Returns axis overlap of local x and local y axes of Object A
bool AxisSeparation(GameObject* obja, GameObject* objb, CollisionInfo *info) {
    RotationMatrix ra = obja->GetReverseRotationMatrix();
    RotationMatrix rb = objb->GetRotationMatrix();
    Vector a_pos = TransformPoint(obja->position, ra);
    float a_half_side = obja->GetSide() / 2;

    Vector b_vertices[4];
    
    for(int i = 0; i < 4; i++) {
        // Get unrotated vertex
        b_vertices[i] = objb->vertices[i];
        // Apply rotation
        b_vertices[i] = TransformPoint(b_vertices[i], rb);
        // Add position to translate to world space
        b_vertices[i] = b_vertices[i] + objb->position;
        // Apply transformation to Object A's axes
        b_vertices[i] = TransformPoint(b_vertices[i], ra);
    }

    float b_min_x = b_vertices[0].x;
    float b_min_y = b_vertices[0].y;
    float b_max_x = b_vertices[0].x;
    float b_max_y = b_vertices[0].y;

    int b_min_x_index = 0;
    int b_max_x_index = 0;
    int b_min_y_index = 0;
    int b_max_y_index = 0;

    // Calculate extrema of vertices
    for(int i = 1; i < 4; i++) {
        if(b_vertices[i].x < b_min_x) {
            b_min_x = b_vertices[i].x;
            b_min_x_index = i;
        }
        if(b_vertices[i].x > b_max_x) {
            b_max_x = b_vertices[i].x;
            b_max_x_index = i;
        }
        if(b_vertices[i].y < b_min_y) {
            b_min_y = b_vertices[i].y;
            b_min_y_index = i;
        }
        if(b_vertices[i].y > b_max_y) {
            b_max_y = b_vertices[i].y;
            b_max_y_index = i;
        }
    }

    // Calculate overlaps
    float x_pos_overlap = (a_pos.x + a_half_side) - b_min_x;
    float x_neg_overlap = b_max_x - (a_pos.x - a_half_side);
    float y_pos_overlap = (a_pos.y + a_half_side) - b_min_y;
    float y_neg_overlap = b_max_y - (a_pos.y - a_half_side);

    // Find minimum overlap
    ra = obja->GetRotationMatrix();
    float min_overlap = min_clamp0(x_pos_overlap, x_neg_overlap, y_pos_overlap, y_neg_overlap);

    if(min_overlap <= 0) return false;

    float mass_factor = (obja->mass + objb->mass) * objb->inv_mass;

    if(min_overlap == x_pos_overlap) {
        Vector normal = ra.GetXNormal();
        info->contact_normal = normal;
        Vector vertex = b_vertices[b_min_x_index];
        Vector point_a_space = vertex - (normal * mass_factor);
        info->contact_point = TransformPoint(point_a_space, ra);
    }
    else if(min_overlap == x_neg_overlap) {
        Vector normal = -(ra.GetXNormal());
        info->contact_normal = normal;
        Vector vertex = b_vertices[b_max_x_index];
        Vector point_a_space = vertex - (normal * mass_factor);
        info->contact_point = TransformPoint(point_a_space, ra);
    }
    else if(min_overlap == y_pos_overlap) {
        Vector normal = ra.GetYNormal();
        info->contact_normal = normal;
        Vector vertex = b_vertices[b_min_y_index];
        Vector point_a_space = vertex - (normal * mass_factor);
        info->contact_point = TransformPoint(point_a_space, ra);
    }
    else if(min_overlap == y_neg_overlap) {
        Vector normal = -(ra.GetYNormal());
        info->contact_normal = normal;
        Vector vertex = b_vertices[b_max_y_index];
        Vector point_a_space = vertex - (normal * mass_factor);
        info->contact_point = TransformPoint(point_a_space, ra);
    }

    info->penetration_depth = min_overlap;

    return true;
}


RotationMatrix GameObject::GetRotationMatrix() {
    float cosa = cos(rotation * PI / 180.0);
    float sina = sin(rotation * PI / 180.0);
    Vector xt = {cosa, -sina};
    Vector yt = {sina, cosa};

    RotationMatrix r = {xt, yt};
    return r;
}


RotationMatrix GameObject::GetReverseRotationMatrix() {
    float cosa = cos(-(rotation * PI / 180.0));
    float sina = sin(-(rotation * PI / 180.0));
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


float min_clamp0(float a, float b) {
    float m = 0;
    if(a <= b) {
        m = a;
    }
    else {
        m = b;
    }
    if(m < 0) return 0;
    else return m;
}


float min_clamp0(float a, float b, float c, float d) {
    return min_clamp0(min_clamp0(a, b), min_clamp0(c, d));
}