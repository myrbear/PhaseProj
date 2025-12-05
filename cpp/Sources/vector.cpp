
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

void p_vec(Vector v) {

	// vector by val
	// print x, y, and z coordinates of a vector;
	// no new line
	
	cout << "< " << v._x << " , " << v._y << " , " << v._z << " >"; 
}

void init_vec(Vector* v) {

	// vector by ref	
	// zero out the data 

	// Vector v; &v
	// Vector* v; v
	
	if (!v) {
	
		cout << "vector is nullptr" << endl;
	}
	
	(*v)._x = 0;
	(*v)._y = 0;
	(*v)._z = 0;
}

void init_vec(Vector* v, float x, float y, float z) {

	// vector by ref
	// fill with values

	(*v)._x = x;
	(*v)._y = y;
	(*v)._z = z;
}

Vector add_vec(Vector a, Vector b) {
	
	// vectors by val
	// add components
	// return sum

	Vector result;

	result._x = a._x + b._x;
	result._y = a._y + b._y;
	result._z = a._z + b._z;

	return result;
}

Vector sub_vec(Vector a, Vector b) {

	// vectors by val
	// subtract components
	// return difference

	Vector result;

	result._x = a._x - b._x;
	result._y = a._y - b._y;
	result._z = a._z - b._z;

	return result;
}

Vector mul_vec(Vector a, float b) {

	// vector by val, float by val
	// multiply float across vector
	
	Vector result;

	result._x = a._x * b;
	result._y = a._y * b;
	result._z = a._z * b;

	return result;
}

Vector div_vec(Vector a, float b) {

	// vector by val, float by val
	// divide float across vector
	
	Vector result;

	result._x = a._x / b;
	result._y = a._y / b;
	result._z = a._z / b;

	return result;
}

float mag(Vector v) {

	// a2 + b2 = c2
	// sqrt( a2 + b2 ) = c
	 
	// vector by val
	// return a scalar, magnitude of vector

	if (v._x == 0 && v._y == 0 && v._z == 0) {

		return 0;
	}

	float x2 = v._x * v._x;
	float y2 = v._y * v._y;
	float z2 = v._z * v._z;

	return sqrt(x2 + y2 + z2);
}

Vector norm(Vector a) {

	// vector by val
	// divide vector by its magnitude
	// return result, a normal, unit length vector

	float m = mag(a);
	
	if (m == 0) {
		
		return a;
	}
	
	return div_vec(a, m);
}

Vector cross(Vector a, Vector b) {
	
	// vector by val, vector by val
	// return the cross product of both vectors
	
	// i     j     k
	// a1    a2    a3
	// b1    b2    b3
	
	// i =   (a2b3 - a3b2)
	// j = - (a1b3 - a3b1)
	// k =   (a1b2 - a2b1)
	
	Vector result;

	result._x =   ( a._y * b._z - a._z * b._y );
	result._y = - ( a._x * b._z - a._z * b._x );
	result._z =   ( a._x * b._y - a._y * b._x );
	
	return result;
}


float dot(Vector a, Vector b) {
	
	// vector by val, vector by val
	// dot product
	// how far along one axis is in context to the other axis
	// commutative, order don't matter
	// multiply component-wise, corresponding components, and then sum
	
	return a._x * b._x + a._y * b._y + a._z * b._z;
}	

