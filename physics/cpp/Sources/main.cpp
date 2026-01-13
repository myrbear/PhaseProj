
#include <iostream>
#include "../Headers/collider.h"

using namespace std;

void p_root(Node* root) {

	cout << "Printing..." << endl;
	cout << endl;
	Node* temp = root;

	while (temp) {
		
		cout << temp->dat._x << endl;
		temp = temp->next;
	}
}

int main() {
   
	Collider col0;
	Collider col1;

	init_col(&col0);
	init_col(&col1);	

	//col1._pos._x = 3;
	col1._pos._y = 1.1;
	col1._pos._x = 1.0;
	//col1._pos._z = 0.5f;
	
	p_col(col0);
	p_col(col1);	
	
	int flag = intersect(&col0, &col1);
	
	cout << "hi ... " << flag << endl;

	return 0;
}
