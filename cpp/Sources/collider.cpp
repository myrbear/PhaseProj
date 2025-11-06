
/*

	author: Myron Lafty
	date: Nov 3, 2025

	This program represents a collider in 3D space.

*/

#include "../Headers/collider.h"
#include <iostream>
#include <stdint.h>

using namespace std;

Collider::Collider() {

}

Vector intersect(Collider* col_ptr) {
	Vector vec;

	vec._x = 10;
	vec._y = 12;
	vec._z = 15;	

	return vec;
}