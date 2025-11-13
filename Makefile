
all: bin/program

bin/program: bin/main.o bin/vector.o bin/collider.o bin/quaternion.o
	g++ -o program bin/main.o bin/vector.o bin/collider.o bin/quaternion.o

bin/main.o: cpp/Sources/main.cpp cpp/Headers/collider.h cpp/Headers/vector.h cpp/Headers/quaternion.h
	g++ -c cpp/Sources/main.cpp -o bin/main.o

bin/collider.o: cpp/Sources/collider.cpp cpp/Headers/collider.h cpp/Headers/vector.h cpp/Headers/quaternion.h
	g++ -c cpp/Sources/collider.cpp -o bin/collider.o

bin/vector.o: cpp/Sources/vector.cpp cpp/Headers/vector.h
	g++ -c cpp/Sources/vector.cpp -o bin/vector.o

bin/quaternion.o: cpp/Sources/quaternion.cpp cpp/Headers/quaternion.h
	g++ -c cpp/Sources/quaternion.cpp -o bin/quaternion.o

