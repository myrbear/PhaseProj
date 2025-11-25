
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
	
	cout << "< " << v._x << " , " << v._y << " , " << v._z << " >"; 
}

void init_vec(Vector* v) {
	
	(*v)._x = 0;
	(*v)._y = 0;
	(*v)._z = 0;
}

void init_vec(Vector* v, float x, float y, float z) {

	(*v)._x = x;
	(*v)._y = y;
	(*v)._z = z;
}

Vector add_vec(Vector a, Vector b) {
	
	Vector result;

	result._x = a._x + b._x;
	result._y = a._y + b._y;
	result._z = a._z + b._z;

	return result;
}

Vector sub_vec(Vector a, Vector b) {

	Vector result;

	result._x = a._x + b._x;
	result._y = a._y + b._y;
	result._z = a._z + b._z;

	return result;
}

Vector mul_vec(Vector a, float b) {
	
	Vector result;

	result._x = a._x * b;
	result._y = a._y * b;
	result._z = a._z * b;

	return result;
}

Vector div_vec(Vector a, float b) {
	
	Vector result;

	result._x = a._x / b;
	result._y = a._y / b;
	result._z = a._z / b;

	return result;
}

float mag(Vector v) {
	
	if (v._x == 0 && v._y == 0 && v._z == 0) {

		return 0;
	}

	float x2 = v._x * v._x;
	float y2 = v._y * v._y;
	float z2 = v._z * v._z;

	return sqrt(x2 + y2 + z2);
}

Vector norm(Vector a) {
	
	float m = mag(a);
	
	if (m == 0) {
		
		return a;
	}
	
	return div_vec(a, m);
}

Vector cross(Vector a, Vector b) {
	
	Vector result;

	result._x =   ( a._y * b._z - a._z * b._y );
	result._y = - ( a._x * b._z - a._z * b._x );
	result._z =   ( a._x * b._y - a._y * b._x );
	
	return result;
}


float dot(Vector a, Vector b) {
	
	return a._x * b._x + a._y * b._y + a._z * b._z;
}	

