#ifndef PHASE_ENGINE
#define PHASE_ENGINE

#include "GameObject.h"
#include <iterator>
#include <cstddef>

#define OBJECT_BUFFER_SIZE 100

class PhaseEngine {
    public:
        PhaseEngine();
        void Run();
        // *GameObject GetObjectBuffer();

        // struct BufferIterator   // For getting game objects
        // {
        //     using iterator_category = std::forward_iterator_tag;
        //     using difference_type   = std::ptrdiff_t;
        //     using value_type        = GameObject;
        //     using pointer           = GameObject*;
        //     using reference         = GameObject&;
        // }
    private:
        GameObject object_buffer[OBJECT_BUFFER_SIZE];
        int id;
};

#endif