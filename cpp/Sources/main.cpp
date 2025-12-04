
#include <iostream>
#include "../Headers/collider.h"

using namespace std;

int main() {
    
	Collider col0;
	Collider col1;

	init_col(&col0);
	init_col(&col1);	

	//col1._pos._x = 3;
	col1._pos._y = 1;
	col1._pos._x = 1;
	//col1._pos._z = 0.5f;

	//p_col(col0);
	//p_col(col1);	
	
	Vector v = intersect(&col0, &col1);

	//p_col(col0);
	//p_col(col1);

	cout << "hi ..." << endl;

	return 0;
}
