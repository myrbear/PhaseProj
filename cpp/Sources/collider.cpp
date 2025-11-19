
/*

	author: Myron Lafty
	date: Nov 3, 2025

	This program represents a collider in 3D space.

*/

#include "../Headers/collider.h"
#include <iostream>
#include <stdint.h>
#include <cfloat>

using namespace std;

int simplex_idx = 0;

void init_col(Collider* col) {
	
	init_vec( &( (*col)._pos      ),  0,  0,  0);
	init_vec( &( (*col)._verts[0] ), -1, -1, -1);
	init_vec( &( (*col)._verts[1] ), -1, -1,  1);
	init_vec( &( (*col)._verts[2] ), -1,  1, -1);
	init_vec( &( (*col)._verts[3] ), -1,  1,  1);
	init_vec( &( (*col)._verts[4] ),  1, -1, -1);
	init_vec( &( (*col)._verts[5] ),  1, -1,  1);
	init_vec( &( (*col)._verts[6] ),  1,  1, -1);
	init_vec( &( (*col)._verts[7] ),  1,  1,  1);
}

Vector** gen_faces(Vector* polytope, int len) {
	
	// take a list of verts, return lists of lists of 3 verts (faces)
	
	Vector** tris = new Vector*[len / 3];

	if (len > 0) {		
		
		for (int i = 0; i < len - 2; i += 3) {
			
			tris[i / 3] = new Vector[3];
			tris[i / 3][0] = polytope[i];
			tris[i / 3][1] = polytope[i + 1];
			tris[i / 3][2] = polytope[i + 2];
		}
	}

	return tris;
}

void delete_faces(Vector** polytope, int len) {

	for (int i = 0; i < len; i++) {
		
		delete[] polytope[i];
	}

	delete[] polytope;
}

Vector get_farthest(Collider col, Vector dir) {

	dir = norm(dir);
	
	int targ_idx = 0;
	float max_dot = FLT_MIN;

	for (int i = 0; i < VERT_COUNT; i++) {
	
		Vector abt_origin = sub_vec(col._verts[i], col._pos);	
		float d = dot(abt_origin, dir);
		
		if (d > max_dot) {
			
			max_dot = d;
			targ_idx = i;
		}
	}

	return col._verts[targ_idx];
}

Vector support(Collider col0, Collider col1, Vector dir) {
	
	Vector n_dir = mul_vec(dir, -1);
	Vector sup0 = get_farthest(col0, dir);
	Vector sup1 = get_farthest(col1, n_dir);
	Vector off = sub_vec(sup0, sup1);

	return off;
}

int line_case(Vector* simplex, Vector* dir) {
	return 1;
}

int tri_case(Vector* simplex, Vector* dir) {
	return 1;
}

int tetra_case(Vector* simplex, Vector* dir) {
	return 1;
}

int simplex_switch(Vector* simplex, Vector* dir) {
	
	switch (simplex_idx) {
		
		case 2:	
			line_case(simplex, dir);
		case 3:
			tri_case(simplex, dir);
		case 4:
			return tetra_case(simplex, dir);
		default:
			break;
	}
	
	return 0;
}

Vector intersect(Collider* col0, Collider* col1) {
		
	Vector result;
	
	init_vec(&result);
	
	int attempts = 100;
	Vector simplex[VERT_COUNT];
	Vector dir = sub_vec((*col0)._pos, (*col1)._pos);
	Vector sup = support(*col0, *col1, dir);
	
	simplex[simplex_idx] = sup;
	simplex_idx++;

	dir = mul_vec(sup, -1);

	for (int i = 0; i < attempts; i++) {
		
		sup = support(*col0, *col1, dir);
		float d = dot(sup, dir);

		if (d <= 0) {
			
			return result;
		}

		simplex[simplex_idx] = sup;
		simplex_idx++;

		if (simplex_switch(&simplex[0], &dir)) {
			
			
		}
	}
	
	cout << simplex[0]._x << endl;
	cout << "test" << endl;	
	
	Vector vec;

	return vec;
}

