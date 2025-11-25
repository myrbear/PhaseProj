
/*

	author: Myron Lafty
	date: Nov 3, 2025

	This program represents a collider in 3D space.

*/

#include "../Headers/collider.h"
#include <iostream>
#include <stdint.h>
#include <cfloat>

#define EPSILON 0.00001f

using namespace std;

int simplex_idx = 0;

void init_col(Collider* col) {
	
	init_vec( &( col->_pos      ),  0,  0,  0);
	init_vec( &( col->_verts[0] ), -1, -1, -1);
	init_vec( &( col->_verts[1] ), -1, -1,  1);
	init_vec( &( col->_verts[2] ), -1,  1, -1);
	init_vec( &( col->_verts[3] ), -1,  1,  1);
	init_vec( &( col->_verts[4] ),  1, -1, -1);
	init_vec( &( col->_verts[5] ),  1, -1,  1);
	init_vec( &( col->_verts[6] ),  1,  1, -1);
	init_vec( &( col->_verts[7] ),  1,  1,  1);
}

void p_col(Collider col) {
	
	for (int i = 0; i < 8; i++) {
	
		p_vec(add_vec(col._pos, col._verts[i]));
	}
	
	cout << endl;
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

void line_case(Vector* simplex, Vector* dir) {

	Vector a = simplex[0];
	Vector b = simplex[1];
	Vector ao = mul_vec(a, -1);
	Vector ab = sub_vec(b, a);

	Vector aoXab = cross(ao, ab);
	Vector abP = cross(ab, aoXab);
	
	(*dir) = abP;
	simplex_idx = 2;
}

void tri_case(Vector* simplex, Vector* dir) {
	
	Vector a = simplex[0];
	Vector b = simplex[1];
	Vector c = simplex[2];
	Vector ao = mul_vec(a, -1);
	Vector ab = sub_vec(b, a);
	Vector ac = sub_vec(c, a);
	Vector abP = cross(ab, cross(ab, ao));
	Vector acP = cross(ac, cross(ac, ao));

	float dt = dot(abP, ao);
	float dt1 = dot(acP, ao);

	if (dt > EPSILON) {
		
		simplex[0] = a;
		simplex[1] = b;
		*dir = abP;
		simplex_idx = 2;
	}
	else if (dt1 > EPSILON) {
		
		simplex[0] = a;
		simplex[1] = c;
		*dir = acP;
		simplex_idx = 2;
	}
	else {
		
		Vector abc = cross(ab, ac);
		float dt2 = dot(abc, ao);
	
		if (dt2 > EPSILON) {
			
			// normal faces origin, swap verts to face away
			// make search direction the unswapped normal
			
			simplex[0] = a;
			simplex[1] = c;
			simplex[2] = b;
			*dir = abc;
			simplex_idx = 3;
		}
		else {
			
			// normal remains pointing away from origin
			// search direction toward origin

			*dir = mul_vec(abc, -1);
			simplex_idx = 3;
		}
	}
}

int tetra_case(Vector* simplex, Vector* dir) {
	
	Vector a = simplex[0];
	Vector b = simplex[1];
	Vector c = simplex[2];
	Vector d = simplex[3];
	
	Vector ba = sub_vec(a, b);
	Vector ca = sub_vec(a, c);
	Vector da = sub_vec(a, d);
	Vector bc = sub_vec(c, b);
	Vector ao = mul_vec(a, -1);
	Vector db = sub_vec(b, d);
	Vector cd = sub_vec(d, c);
	
	// so long as the ordder of the original triangle has a normal that faces
	// away from the origin
	// the order will permit
	// these three normals will face away from each other
	// tested

	Vector bac = cross(ba, bc);
	Vector cad = cross(ca, cd);
	Vector dab = cross(da, db);
	
	float bacDot = dot(bac, ao);
	float cadDot = dot(cad, ao);
	float dabDot = dot(dab, ao);
	
	// bc x bd is previous step's ab x ac, so it is skipped
	// bc x bd will face away from the origin

	if (bacDot > EPSILON) {
		
		simplex[0] = b;
		simplex[1] = c;
		simplex[2] = a;
		*dir = bac;
		simplex_idx = 3;
		return 0;
	}
	else if (cadDot > EPSILON) {
		
		simplex[0] = c;
		simplex[1] = d;
		simplex[2] = a;
		*dir = cad;
		simplex_idx = 3;
		return 0;
	}
	else if (dabDot > EPSILON) {
		
		simplex[0] = d;
		simplex[1] = b;
		simplex[2] = a;
		*dir = dab;
		simplex_idx = 3;
		return 0;
	}

	// all faces produce normals away from the origin

	return 1;
}

int simplex_switch(Vector* simplex, Vector* dir) {
	
	switch (simplex_idx) {
		
		case 2:	
			line_case(simplex, dir);
			cout << "line case passed" << endl;
			cout << "simplex: ";
			p_vec(simplex[0]);
			cout << " , ";
			p_vec(simplex[1]);
			cout << endl;
		case 3:
			tri_case(simplex, dir);
			cout << "tri case passed" << endl;
			cout << "simplex: " << simplex[0]._x << ", " << simplex[1]._x  << ", " << simplex[2]._x << endl;
		case 4:
			cout << "tetra case passed" << endl;
			cout << "simplex: " << simplex[0]._x << ", " << simplex[1]._x << ", " << simplex[2]._x << ", " << simplex[3]._x << endl;
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
	Vector dir = sub_vec(col0->_pos, col1->_pos);
	Vector sup = support(*col0, *col1, dir);

	
                cout << "dir: ";
                p_vec(dir);
                cout << endl;
	
	cout << "sup: ";	
	p_vec(sup);  
	cout << endl;
	
	init_vec(&simplex[0]);
	simplex_idx = 1;

	dir = mul_vec(sup, -1);

	for (int i = 0; i < attempts; i++) {
		
		sup = support(*col0, *col1, dir);
		float d = dot(sup, dir);

		cout << "sup: ";
		p_vec(sup);
		cout << endl;

		cout << "dir: ";
		p_vec(dir);
		cout << endl;

		if (d <= 0) {
			cout << "d <= 0" << endl;	
			return result;
		}
		
		simplex[simplex_idx] = sup;
		simplex_idx++;

		if (simplex_switch(&simplex[0], &dir)) {
						
			Vector ret;
			init_vec(&ret);
			ret._x = 1;
			return ret;	
		}
	}
	
	cout << simplex[0]._x << endl;
	cout << "test" << endl;	
	
	Vector vec;

	init_vec(&vec);

	return vec;
}

