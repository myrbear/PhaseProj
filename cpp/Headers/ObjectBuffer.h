#ifndef OBJECT_BUFFER
#define OBJECT_BUFFER

#include "../Headers/GameObject.h"
#include <iterator>
#include <cstddef>
#include <mutex>
#include <shared_mutex>

#define OBJECT_BUFFER_SIZE 100
#define READ 0
#define WRITE 1

using namespace std;

class ObjectBuffer {
    public:
        ObjectBuffer();
        
        // Custom Iterator
        struct ObjectIterator
        {
            using iterator_category = forward_iterator_tag;
            using difference_type   = ptrdiff_t;
            using value_type        = GameObject*;
            using pointer           = GameObject**;
            using reference         = GameObject*&;

            public:
                ObjectIterator(ObjectBuffer* _ptr, int _index, int _rw, shared_mutex* _mtx) : ptr(_ptr), index(_index), rw(_rw), mtx(_mtx) {}

                // Read lock on mutex
                GameObject* operator*() const {
                    shared_lock lock(*mtx);
                    GameObject** buffer = NULL;
                    if(rw == READ) {
                        buffer = ptr->read_buffer_ptr;
                    }
                    else if(rw == WRITE) {
                        buffer = ptr->write_buffer_ptr;
                    }
                    return buffer[index];
                }
                GameObject** operator->() {
                    shared_lock lock(*mtx);
                    GameObject** buffer = NULL;
                    if(rw == READ) {
                        buffer = ptr->read_buffer_ptr;
                    }
                    else if(rw == WRITE) {
                        buffer = ptr->write_buffer_ptr;
                    }
                    return buffer + index;
                }

                // Prefix increment
                ObjectIterator& operator++() {
                    index++;
                    GameObject** buffer = NULL;
                    if(rw == READ) {
                        buffer = ptr->read_buffer_ptr;
                    }
                    else if(rw == WRITE) {
                        buffer = ptr->write_buffer_ptr;
                    }
                    // Skip NULL pointers
                    while(index != OBJECT_BUFFER_SIZE && buffer[index] == NULL) {
                        index++;
                    }
                    return *this;
                } 

                // Postfix increment
                ObjectIterator operator++(int) { ObjectIterator tmp = *this; ++(*this); return tmp; }

                friend bool operator== (const ObjectIterator& a, const ObjectIterator& b) { return a.index == b.index; };
                friend bool operator!= (const ObjectIterator& a, const ObjectIterator& b) { return a.index != b.index; }; 

            private:
                ObjectBuffer* ptr;  // Pointer to GameObject array
                int index;
                int rw;
                shared_mutex* mtx;
        };

        ObjectIterator read_begin() { return ObjectIterator(this, 0, READ, &mtx); }
        ObjectIterator read_end() { return ObjectIterator(this, OBJECT_BUFFER_SIZE, READ, &mtx); }
        ObjectIterator write_begin() { return ObjectIterator(this, 0, WRITE, &mtx); }
        ObjectIterator write_end() { return ObjectIterator(this, OBJECT_BUFFER_SIZE, WRITE, &mtx); }

        // Functions
        int CreateObject();
        bool DeleteObject(int id);
        bool Full();
        void SwapBuffers();

        shared_mutex mtx;

        GameObject* bufferA[OBJECT_BUFFER_SIZE] = { NULL };
        GameObject* bufferB[OBJECT_BUFFER_SIZE] = { NULL };

        GameObject** read_buffer_ptr;
        GameObject** write_buffer_ptr;

    private:
        int buffer_index = 0;
};

#endif // OBJECT_BUFFER