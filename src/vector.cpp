
/*

	author: Myron Lafty
	date: Nov 3, 2025

	This program represents a vector in 3D space.

*/

#include <iostream>
#include <stdint.h>
#include <vector.h>

using namespace std;

Vector init_vec() {

	Vector vec;

	vec._x = 0;
	vec._y = 0;
	vec._z = 0;

	return vec;
}

Vector cross(Vector* a, Vector* b) {
	
	Vector result;

	result._x =   ( a._y * b._z - a._z * b._y );
	result._y = - ( a._x * b._z - a._z * b._x );
	result._z =   ( a._x * b._y - a._y * b._x );
	
	return result;
}
