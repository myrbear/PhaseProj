#ifndef OBJECT_BUFFER
#define OBJECT_BUFFER

#include "../Headers/GameObject.h"
#include <iostream>
#include <iterator>
#include <cstddef>
#include <mutex>
#include <shared_mutex>

#define OBJECT_BUFFER_SIZE 100
#define READ 0
#define WRITE 1
// Object Change Types
#define OBJECT_CREATE 0
#define OBJECT_DELETE 1
#define POSITION_SET 2
#define ROTATION_SET 3
#define VELOCITY_SET 4
#define POSITION_ADD 5
#define ROTATION_ADD 6
#define VELOCITY_ADD 7


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

        ObjectIterator read_begin() { ObjectIterator it(this, 0, READ, &mtx); if(*it == NULL) it++; return it; }
        ObjectIterator read_end() { return ObjectIterator(this, OBJECT_BUFFER_SIZE, READ, &mtx); }
        ObjectIterator write_begin() { ObjectIterator it(this, 0, WRITE, &mtx); if(*it == NULL) it++; return it; }
        ObjectIterator write_end() { return ObjectIterator(this, OBJECT_BUFFER_SIZE, WRITE, &mtx); }

        // Functions
        int CreateObject();
        void DeleteObject(int id);
        GameObject GetGameObject(int id);
        void SetPosition(int id, float x, float y, float z);
        void SetRotation(int id, float x, float y, float z, float w);
        void SetVelocity(int id, float vx, float vy);
        void AddPosition(int id, float dx, float dy, float dz);
        void AddRotation(int id, float dx, float dy, float dz, float dw);
        void AddVelocity(int id, float dvx, float dvy);
        bool Full();
        void SwapBuffers();
        void ApplyChanges();

        shared_mutex mtx;

        GameObject* bufferA[OBJECT_BUFFER_SIZE] = { NULL };
        GameObject* bufferB[OBJECT_BUFFER_SIZE] = { NULL };

        GameObject** read_buffer_ptr;
        GameObject** write_buffer_ptr;

    private:
        int buffer_index = 0;

        // Change Queue
        // Only apply changes to GameObjects outside of physics calculations
        struct ObjectChangeNode {
            public:
                ObjectChangeNode(int _ct, int _id, int _v1=0, int _v2 = 0, int _v3 = 0, int _v4 = 0) : change_type(_ct), id(_id), val1(_v1), val2(_v2), val3(_v3), val4(_v4) {}
                int change_type;
                int id;
                float val1;
                float val2;
                float val3;
                float val4;
                ObjectChangeNode* next = NULL;
        };
        ObjectChangeNode* head = NULL;
        ObjectChangeNode* tail = NULL;
        void Enqueue(ObjectChangeNode* node);
        ObjectChangeNode* Dequeue();
        mutex queue_mtx;
};

#endif // OBJECT_BUFFER