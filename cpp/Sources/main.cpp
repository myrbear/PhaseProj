
#include <iostream>
#include "../Headers/collider.h"

using namespace std;

int main(void) {
	
	Collider col0;
	Collider col1;

	init_col(&col0);
	init_col(&col1);	

	col1._pos._x = 0.5f;
	col1._pos._y = 0.5f;
	col1._pos._z = 0.5f;

	p_col(col0);
	p_col(col1);	
	
	Vector v = intersect(&col0, &col1);

	cout << v._x << " " << v._y << " " << v._z << endl;
	cout << "hi ..." << endl;

	return 0;
}
