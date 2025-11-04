#ifndef OBJECT_BUFFER
#define OBJECT_BUFFER

#include "../Headers/GameObject.h"
#include <iterator>
#include <cstddef>

#define OBJECT_BUFFER_SIZE 100

class ObjectBuffer {
    public:

        // Custom Iterator
        struct ObjectIterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = GameObject*;
            using pointer           = GameObject**;
            using reference         = GameObject*&;

            public:
                ObjectIterator(GameObject** _ptr, GameObject** _end_ptr) : ptr(_ptr), end_ptr(_end_ptr) {}

                GameObject* operator*() const { return *ptr; }
                GameObject* operator->() { return *ptr; }

                // Prefix increment
                ObjectIterator& operator++() {
                    ptr++;
                    // Skip NULL pointers
                    while(ptr != end_ptr && *ptr == NULL) {
                        ptr++;
                    }
                    return *this;
                } 

                // Postfix increment
                ObjectIterator operator++(int) { ObjectIterator tmp = *this; ++(*this); return tmp; }

                friend bool operator== (const ObjectIterator& a, const ObjectIterator& b) { return a.ptr == b.ptr; };
                friend bool operator!= (const ObjectIterator& a, const ObjectIterator& b) { return a.ptr != b.ptr; }; 

            private:
                GameObject** ptr;
                GameObject** end_ptr;
        };

        ObjectIterator begin() { return ObjectIterator(&buffer_array[0], &buffer_array[OBJECT_BUFFER_SIZE]); }
        ObjectIterator end() { return ObjectIterator(&buffer_array[OBJECT_BUFFER_SIZE], &buffer_array[OBJECT_BUFFER_SIZE]); }

        // Functions
        bool Add(GameObject* ptr);
        bool Remove(GameObject* ptr);
        bool Full();

    private:
        GameObject* buffer_array[OBJECT_BUFFER_SIZE] = { NULL };
        int buffer_index = 0;
};

#endif // OBJECT_BUFFER