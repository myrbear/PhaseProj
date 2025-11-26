
#include <iostream>
#include "../Headers/collider.h"

using namespace std;

int main() {
    
	Node* root = nullptr;
    Vector tmp;

    // Insert some vectors
    init_vec(&tmp, 1, 2, 3);
    root = insert(root, tmp, 0); // head

    init_vec(&tmp, 4, 5, 6);
    root = insert(root, tmp, 1); // tail

    init_vec(&tmp, 7, 8, 9);
    root = insert(root, tmp, 1); // middle

    // Traverse and print
    for (int i = 0; i < 3; ++i) {
        Vector v = traverse(root, i);
        std::cout << "Node " << i << ": ";
        p_vec(v);
    }

    // Set node at index 1
    init_vec(&tmp, 10, 11, 12);
    set_node(root, tmp, 1);

    std::cout << "After set_node at index 1:\n";
    for (int i = 0; i < 3; ++i) {
        Vector v = traverse(root, i);
        std::cout << "Node " << i << ": ";
        p_vec(v);
    }

    // Remove head
    root = remove(root, 1);

    std::cout << "After removing 1:\n";
    for (int i = 0; i < 2; ++i) {
        Vector v = traverse(root, i);
        std::cout << "Node " << i << ": ";
        p_vec(v);
    }

    // Clear the list
    root = clear(root);
    if (!root) std::cout << "List cleared successfully.\n";

    return 0;

	
	Collider col0;
	Collider col1;

	init_col(&col0);
	init_col(&col1);	

	//col1._pos._x = 3;
	col1._pos._y = 5.5f;
	//col1._pos._z = 0.5f;

	p_col(col0);
	p_col(col1);	
	
	Vector v = intersect(&col0, &col1);

	cout << v._x << " " << v._y << " " << v._z << endl;
	cout << "hi ..." << endl;

	return 0;
}
