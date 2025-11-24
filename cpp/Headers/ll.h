
#include <stdint.h>
#include "vector.h"

struct Node {

	Vector dat;
	Node* next;
};

Node* insert(Node* root, Vector dat, int loc);
Node* remove(Node* root, int loc);

