
/*

	author: Myron Lafty
	date: Oct 28, 2025

*/

#include <iostream>
#include "../Headers/ll.h"

using namespace std;

Node* clear(Node* root) {
	
	Node* temp = root;

	while (temp) {
		
		Node* next = temp->next;

		delete temp;

		temp = next;
	}
	
	root = nullptr;

	return root;
}

Vector traverse(Node* root, int loc) {
	
	int idx = 0;
	Node* temp = root;

	while (temp && idx < loc) {
		
		temp = temp->next;
		idx++;	
	}
	
	if (temp && idx == loc) {
		
		return temp->dat;
	}

	Vector v;
	init_vec(&v);

	return v;
}

void set_node(Node* root, Vector dat, int loc) {
	
	if (!root) {
		
		return;
	}

	int idx = 0;
	Node* cur = root;

	while (cur && idx < loc) {
		
		cur = cur->next;
		idx++;
	}

	if (!cur) {
		
		// if loc is larger than list size, cur will end up as nullptr

		return;
	}

	// data is a Vector, not a Vector*
	// it belongs to the struct Node cur

	cur->dat = dat;
}

Node* insert(Node* root, Vector dat, int loc) {
	
	if (loc < 0) {
		
		// do nothing

		return root;
	}

	int idx = 0;
	Node* cur = root;
	Node* prev = nullptr;

	while (cur && idx < loc) {
		
		prev = cur;
		cur = cur->next;
		idx++;
	}	

	// always inserting Node* in
	// in next might be nullptr

	Node* in = new Node();
	in->dat = dat;

	if (cur == root) {
		
		in->next = cur;
	}
	else {
	
		in->next = nullptr;
	}

	if (prev) {

		prev->next = in;
		return root;
	}
	else {
		
		// prev is null
		// therefore loc is 0
		// or root is nullptr
		// return new head Node* in
		return in;
	}
}

Node* remove(Node* root, int loc) {

	if (loc < 0 || !root) {
		
		return root;
	}

	Node* prev = nullptr;
	Node* cur = root;
	int idx = 0;
	
	while (cur && idx < loc) {
		
		prev = cur;
		cur = cur->next;
		idx++;
	}

	if (!cur) {
		
		// root is null
		return root;
	}
	
	if (prev) {
		
		// root is not null
		// loc is greater than 0
		// loc may be greater than size

		prev->next = cur->next;
	}
	else {
		
		// loc is 0
		root = cur->next;
	}

	// return new head

	delete cur;
	return root;
}

