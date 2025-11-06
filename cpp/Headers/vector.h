
#ifndef VECTOR_H
#define VECTOR_H

class Vector {
	public:
		float _x;
		float _y;
		float _z;

		Vector();
		Vector(int x, int y, int z);
		Vector cross();
		float dot();
};

#endif // VECTOR_H
