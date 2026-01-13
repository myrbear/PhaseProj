CC = g++
CFLAGS = -Wall -O2
LDFLAGS = -lwinmm -pthread -lraylib -lopengl32 -lgdi32

# List all PHASE source files here
PHASE_SRC = PhaseEngine.cpp \
		GameObject.cpp \
		ObjectBuffer.cpp

# Automatically generate PHASE object files
PHASE_OBJ = $(PHASE_SRC:%.cpp=%.o)
PHASE_BIN = $(PHASE_SRC:%.cpp=bin/%.o)

TARGET =  phase_editor.out

all: $(TARGET)

$(TARGET): engine main.o
	$(CC) bin/main.o $(PHASE_BIN) -o $(TARGET) $(LDFLAGS)

engine: $(PHASE_OBJ)

# Compile each .cpp file into .o
%.o: cpp/Sources/%.cpp
	$(CC) -c $< -o bin/$@ $(CFLAGS)

# Clean up object files and executable
clean:
	rm -f bin/$(PHASE_OBJ)
