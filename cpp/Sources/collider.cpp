
/*

	author: Myron Lafty
	date: Nov 3, 2025

	This program represents a collider in 3D space.

*/

#include <iostream>
#include <stdint.h>

using namespace std;

const uint8_t VERT_COUNT = 10;

struct Vector {
	float _x;
	float _y;
	float _z;
};

struct Collider {
	Vector _pos;
	Vector _verts[VERT_COUNT];
};

Vector init_vec() {
	Vector vec;

	vec._x = 0;
	vec._y = 0;
	vec._z = 0;

	return vec;
}

Vector intersect(Collider* col_ptr) {
	Vector vec;

	vec._x = 10;
	vec._y = 12;
	vec._z = 15;	

	return vec;
}

int main(void) {
	
	Collider col;

	col._pos._x = 10;
	col._pos._y = 11;
	col._pos._z = 12;
	
	cout << col._pos._x << endl;	

	return 0;
}
