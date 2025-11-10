
/*

	author: Myron Lafty
	date: Nov 3, 2025

	This program represents a vector in 3D space.

*/

#include <iostream>
#include <stdint.h>
#include "../Headers/vector.h"
#include <cmath>

using namespace std;

Vector::Vector() : _x(0), _y(0), _z(0) {}

Vector::Vector(int x, int y, int z) : _x(x), _y(y), _z(z) {}

Vector mul(Vector* a, float b) {
	
	Vector result;

	result._x = a->_x * b;
	result._y = a->_y * b;
	result._z = a->_z * b;

	return result;
}

Vector div(Vector* a, float b) {
	
	Vector result;

	result._x = a->_x / b;
	result._y = a->_y / b;
	result._z = a->_z / b;

	return result;
}

float mag(Vector* a) {
	
	if (a->_x == 0 && a->_y == 0 && a->_z == 0) {

		return 0;
	}

	float x2 = a->_x * a->_x;
	float y2 = a->_y * a->_y;
	float z2 = a->_z * a->_z;

	return sqrt(x2 + y2 + z2);
}

Vector norm(Vector* a) {
	
	float m = mag(a);
	
	if (m == 0) {
		
		Vector* vec = new Vector();
			
		return (*vec);
	}
	
	return div(a, m);
}

Vector cross(Vector* a, Vector* b) {
	
	Vector result;

	result._x =   ( a->_y * b->_z - a->_z * b->_y );
	result._y = - ( a->_x * b->_z - a->_z * b->_x );
	result._z =   ( a->_x * b->_y - a->_y * b->_x );
	
	return result;
}


float dot(Vector* a, Vector* b) {
	
	return a->_x * b->_x + a->_y * b->_y + a->_z * b->_z;
}	
