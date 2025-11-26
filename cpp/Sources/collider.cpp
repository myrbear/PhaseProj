
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

	// initialize a collider
	// colliders have position, rotation, scale, and vertices
	// currently, position and vertices are the only relevant properties

	// init a collider at pos <0,0,0>
	// give it 4 vertices because we're in 2D space.
	// the result is a square centered at the origin where each side is 2 units
	
	init_vec(&col->_pos,  0,  0,  0);
	
	Vector dat0;
	Vector dat1;
	Vector dat2;
	Vector dat3;
	
	init_vec(&dat0, -1, -1,  0);
	init_vec(&dat1, -1,  1,  0);
	init_vec(&dat2,  1, -1,  0);
	init_vec(&dat3,  1,  1,  0);

	col->_verts = insert(col->_verts, dat0, 0);
	col->_verts = insert(col->_verts, dat1, 1);
	col->_verts = insert(col->_verts, dat2, 2);
	col->_verts = insert(col->_verts, dat3, 3);	
}

void p_col(Collider col) {
	
	// print the vertices of a collider in world space
	// (adds position)

	Node* temp = col._verts;

	while (temp) {
		
		Vector v = temp->dat;
		Vector sum = add_vec(col._pos, v);

		p_vec(sum);

		temp = temp->next;
	}
	
	cout << endl;
}

Vector** gen_faces(Node* polytope, int len) {
	
	// take a list of verts, return lists of lists of 3 verts (faces)
	
	Vector** tris = new Vector*[len / 3];

	if (len > 0) {		
		
		for (int i = 0; i < len - 2; i += 3) {
			
			tris[i / 3] = new Vector[3];
			tris[i / 3][0] = traverse(polytope, i);
			tris[i / 3][1] = traverse(polytope, i + 1);
			tris[i / 3][2] = traverse(polytope, i + 2);
		}
	}

	return tris;
}

void delete_faces(Vector** polytope, int len) {

	// faces are constructed with the new operator
	// delete each array in faces to protect memory

	for (int i = 0; i < len; i++) {
		
		delete[] polytope[i];
	}

	delete[] polytope;
}

Vector get_farthest(Collider col, Vector dir) {

	// given a direction
	// given the position of a collider
	// given the vertices of a collider

	// assume direction starts from the origin
	// find the vector that is farthest in given direction (use dot product)
	// then return that vector in world space by adding position

	dir = norm(dir);
	
	int targ_idx = 0;
	int i = 0;
	float max_dot = FLT_MIN;

	Node* temp = col._verts;

	while (temp) {

		Vector abt_origin = sub_vec(temp->dat, col._pos);	
		float d = dot(abt_origin, dir);
		
		if (d > max_dot) {
			
			max_dot = d;
			targ_idx = i;
		}

		i++;
		temp = temp->next;
	}

	Vector targ_vec = traverse(col._verts, targ_idx);
	Vector sum = add_vec(col._pos, targ_vec);

	return sum;
}

Vector support(Collider col0, Collider col1, Vector dir) {

	// given a search direction and two colliders
	// find the farthest point on one collider in the given direction
	// find the farthest point on the other collider in the opposite direction
	// get both farthest points in world space
	// return the offset between those points
	// this offset will be treated as a point (same offset, but starts at origin) and will be added to the simplex

	Vector n_dir = mul_vec(dir, -1);
	Vector sup0 = get_farthest(col0, dir);
	
	cout << "farthest A: ";
	p_vec(sup0);
	cout << endl;
	
	Vector sup1 = get_farthest(col1, n_dir);
	
	cout << "farthest B: ";
	p_vec(sup1);
	cout << endl;	

	Vector off = sub_vec(sup0, sup1);

	return off;
}

void line_case(Node* simplex, Vector* dir) {

	// there are two points on the simplex
	// create an offset between them (edge ab)
	// create an offset between the first point and the origin (ao)
	// cross ao and ab to get aoXab then cross ab with aoXab again
	// the first cross will face up or down relative to the plane of ao and ab
	// then by crossing ab with aoXab again, we'll get a vector perpendicular to ab
	// that perpendicular vector will either be facing the origin or facing away from the origin
	// conceptually, the perpendicular vector starts at point a
	
	Vector a = traverse(simplex, 0);
	Vector b = traverse(simplex, 1);
	Vector ao = mul_vec(a, -1);
	Vector ab = sub_vec(b, a);

	Vector aoXab = cross(ao, ab);
	Vector abP = cross(ab, aoXab);
	
	(*dir) = abP;
}

int tri_case(Node* simplex, Vector* dir) {
	
	// same concept as the line case
	// now we have 3 points
	// since ab from the line case was already tested, we can skip that edge
	// so we need ab perp, ac perp, current cb is the same edge as last step's ab
	
	Vector a = traverse(simplex, 0);
	Vector b = traverse(simplex, 1);
	Vector c = traverse(simplex, 2);
	Vector ao = mul_vec(a, -1);
	Vector ab = sub_vec(b, a);
	Vector ac = sub_vec(c, a);
	Vector abP = cross(ab, cross(ab, ao));
	Vector acP = cross(ac, cross(ac, ao));

	float dt = dot(abP, ao);
	float dt1 = dot(acP, ao);

	if (dt > EPSILON) {
		
		clear(simplex);
		insert(simplex, a, 0);
		insert(simplex, b, 1);
		(*dir) = abP;
	}
	else if (dt1 > EPSILON) {
		
		clear(simplex);
		insert(simplex, a, 0);
		insert(simplex, c, 1);
		(*dir) = acP;
	}
	else {
		
		Vector abc = cross(ab, ac);
		float dt2 = dot(abc, ao);
	
		if (dt2 > EPSILON) {
			
			// normal faces origin, swap verts to face away
			// make search direction the unswapped normal
			
			clear(simplex);
			insert(simplex, a, 0);
			insert(simplex, c, 1);
			insert(simplex, b, 2);
			(*dir) = abc;
			
			return 1;
		}
		else {
			
			// normal remains pointing away from origin
			// search direction toward origin

			(*dir) = mul_vec(abc, -1);
			
			return 1;
		}
	}

	return 0;
}

int tetra_case(Node* simplex, Vector* dir) {
	
	Vector a = traverse(simplex, 0);
	Vector b = traverse(simplex, 1);
	Vector c = traverse(simplex, 2);
	Vector d = traverse(simplex, 3);
	
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
	
		clear(simplex);
		insert(simplex, b, 0);
		insert(simplex, c, 1);
		insert(simplex, a, 2);
		*dir = bac;
	
		return 0;
	}
	else if (cadDot > EPSILON) {
                
		clear(simplex);
                insert(simplex, c, 0);
                insert(simplex, d, 1);
                insert(simplex, a, 2);		
		*dir = cad;
		
		return 0;
	}
	else if (dabDot > EPSILON) {
		
		clear(simplex);
                insert(simplex, d, 0);
                insert(simplex, b, 1);
                insert(simplex, a, 2);
		*dir = dab;
		
		return 0;
	}

	// all faces produce normals away from the origin

	return 1;
}

int simplex_switch(Node* simplex, Vector* dir) {
	
	Node* temp = simplex;
	
	while (temp) {
		
		p_vec(temp->dat);
		temp = temp->next;
	}

	switch (simplex_idx) {
		
		case 2:
			cout << "line case running" << endl;	
			line_case(simplex, dir);
		case 3:
			cout << "tri case running" << endl;
			return tri_case(simplex, dir);
		case 4:
			cout << "tetra case running" << endl;
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
	Node* simplex;
	Vector dir = sub_vec(col0->_pos, col1->_pos);
	Vector sup = support(*col0, *col1, dir);

	
                cout << "dir: ";
                p_vec(dir);
                cout << endl;
	
	cout << "sup: ";	
	p_vec(sup);  
	cout << endl;
	
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

		cout << "dot: " << d << endl;

		if (d <= 0) {
			cout << "d <= 0" << endl;	
			return result;
		}

		insert(simplex, sup, 0);

		if (simplex_switch(simplex, &dir)) {
						
			Vector ret;
			init_vec(&ret);
			ret._x = 1;
			return ret;	
		}
	}
	
	cout << traverse(simplex, 0)._x << endl;
	cout << "test" << endl;	
	
	Vector vec;

	init_vec(&vec);

	return vec;
}

