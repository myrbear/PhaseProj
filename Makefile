CC = g++
CFLAGS = -Wall -O2

# List all source files here
SRC = 	PhaseEngine.cpp \
		GameObject.cpp \
		ObjectBuffer.cpp \
		collider.cpp \
		vector.cpp \
		quaternion.cpp

# Automatically generate object files
OBJ = $(SRC:%.cpp=bin/%.o)

all: $(OBJ)

# Compile each .cpp file into .o
bin/%.o: cpp/Sources/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)