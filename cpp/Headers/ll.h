
#ifndef LL_H
#define LL_H

#include <stdint.h>
#include "vector.h"

struct Node {

	Vector dat;
	Node* next;
};

Node* clear(Node* root);
Vector traverse(Node* root, int loc);
Node* insert(Node* root, Vector dat, int loc);
void set_node(Node* root, Vector dat, int loc);
Node* remove(Node* root, int loc);

#endif // LL_H
