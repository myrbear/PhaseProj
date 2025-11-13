
#include <iostream>
#include "../Headers/collider.h"

using namespace std;

int main(void) {
	
	Collider col0;
	Collider col1;

	init_col(&col0);
	init_col(&col1);

	intersect(&col0, &col1);

	cout << col0._verts[0]._x << endl;

	cout << "hi" << endl;
	return 0;
}
