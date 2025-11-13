
#ifndef VECTOR_H
#define VECTOR_H

struct Vector {

	float _x;
	float _y;
	float _z;
};

void init_vec(Vector* v);
void init_vec(Vector* v, float x, float y, float z);
float dot(Vector a, Vector b);
Vector cross(Vector a, Vector b);
Vector mul_vec(Vector a, float b);
Vector div_vec(Vector a, float b);
Vector sub_vec(Vector a, Vector b);
Vector add_vec(Vector a, Vector b);
float mag(Vector a);
Vector norm(Vector a);

#endif // VECTOR_H
