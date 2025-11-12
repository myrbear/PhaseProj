
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
	
	_pos = new Vector(0, 0, 0);
	_verts[0] = new Vector(-1, -1, -1);
	_verts[1] = new Vector(-1, -1, 1);
	_verts[2] = new Vector(-1, 1, -1;
	_verts[3] = new Vector(-1, 1, 1);
	_verts[4] = new Vector(1, -1, -1);
	_verts[5] = new Vector(1, -1, 1);
	_verts[6] = new Vector(1, 1, -1);
	_verts[7] = new Vector(1, 1, 1);
}

Vector[][] getFaces(Vector* polytope, int len) {
	
	// take a list of verts, return lists of lists of 3 verts (faces)

	Vector tris[len / 3][3];

	if (len > 0) {
	
		for (int i = 0; i < len - 2; i += 3) {
			
			tris[i / 3][0] = polytope[i];
			tris[i / 3][1] = polytope[i + 1];
			tris[i / 3][2] = polytope[i + 2];
		}
	}

	return tris;
}

Vector Collider::getFarthest(Vector* dir) {

	*dir = norm(dir);
	
	int targ_idx = 0;
	float max_dot = MIN_FLOAT;

	for (int i = 0; i < VERT_COUNT; i++) {
		Vector abt_origin = minus(_verts[i], _pos);
		
		float d = dot(&abt_origin, dir);
		
		if (d > max_dot) {
			
			max_dot = d;
			targ_idx = i;
		}
	}

	return _verts[targ_idx];
}

Vector Collider::support(Vector* dir, Collider* col) {
	
	Vector sup = getFarthest(dir);
	Vector n_dir = mul(dir, -1);
	Vector off = minus(sup, col->getFarthest(n_dir));

	return off;
}

Vector Collider::intersect(Collider* col) {
		
	Vector result = *(new Vector(0, 0, 0));
	int attempts = 100;
	Vector simplex[VERT_COUNT];
	Vector dir = minus(col->_pos, _pos);
	Vector sup = support(&dir, col);
	
	simplex[0] = sup;

	dir = mul(sup, -1);

	for (int i = 0; i < attempts; i++) {
		
		sup = support(&dir, col);

		float d = dot(&sup, &dir);

		if (d <= 0) {
			
			return result;
		}

		simplex[0] = sup;
	}

	return vec;
}
