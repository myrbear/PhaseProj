#include "../Headers/ObjectBuffer.h"


ObjectBuffer::ObjectBuffer() {
    read_buffer_ptr = bufferA;
    write_buffer_ptr = bufferB;
}


int ObjectBuffer::CreateObject() {
    if(buffer_index == OBJECT_BUFFER_SIZE) {
        // Object buffer full, return invalid Id
        return -1;
    }
    else {
        // New Id is buffer index
        int new_id = buffer_index;
        // Allocate memory for two objects
        GameObject* read_obj = new GameObject(new_id);
        GameObject* write_obj = new GameObject(new_id);
        // Insert object pointer
        read_buffer_ptr[buffer_index] = read_obj;
        write_buffer_ptr[buffer_index] = write_obj;

        // Increment index
        while(buffer_index < OBJECT_BUFFER_SIZE && read_buffer_ptr[buffer_index] != NULL) {
            buffer_index++;
        }

        // Object added, return id
        return new_id;
    }
}


bool ObjectBuffer::DeleteObject(int id) {
    // Find object
    GameObject* read_obj = read_buffer_ptr[id];
    GameObject* write_obj = write_buffer_ptr[id];

    if(read_obj == NULL) {
        // No object of this id
        return false;
    }

    delete read_obj;
    delete write_obj;

    return true;
}


bool ObjectBuffer::Full() {
    return buffer_index == OBJECT_BUFFER_SIZE;
}


void ObjectBuffer::SetPosition(int id, float x, float y, float z) {
    // Enqueue change
    ObjectChangeNode* node = new ObjectChangeNode(POSITION_SET, id, x, y, z);
    Enqueue(node);
}


void ObjectBuffer::SetRotation(int id, float x, float y, float z, float w) {
    ObjectChangeNode* node = new ObjectChangeNode(ROTATION_SET, id, x, y, z, w);
    Enqueue(node);
}


void ObjectBuffer::AddPosition(int id, float dx, float dy, float dz) {
    ObjectChangeNode* node = new ObjectChangeNode(POSITION_ADD, id, dx, dy, dz);
    Enqueue(node);
}


// Saftely swaps the read and write buffer, writes read buffer to write buffer
void ObjectBuffer::SwapBuffers() {
    // Write lock on the mutex
    {
        unique_lock lock(mtx);

        // Swap buffer pointers
        auto temp = read_buffer_ptr;
        read_buffer_ptr = write_buffer_ptr;
        write_buffer_ptr = temp;
    }

    // Write the objects in the read buffer to the write buffer
    for(int i = 0; i < OBJECT_BUFFER_SIZE; i++) {
        GameObject* read_obj = read_buffer_ptr[i];
        GameObject* write_obj = write_buffer_ptr[i];

        if(read_obj == NULL) {
            continue;
        }

        *write_obj = *read_obj;
    }
}


// Dequeue all changes and apply them to the objects
void ObjectBuffer::ApplyChanges() {
    // Dequeue until empty
    while(head != NULL) {
        ObjectChangeNode* node = Dequeue();

        // Check Id validity
        if(node->id < 0 || node->id >= OBJECT_BUFFER_SIZE || write_buffer_ptr[node->id] == NULL) {
            cout << "ERROR: invalid GameObject Id" << endl;
            delete node;
            continue;
        }

        GameObject* obj = write_buffer_ptr[node->id];

        switch(node->change_type) {
            case POSITION_SET:
                obj->collider._pos._x = node->val1;
                obj->collider._pos._y = node->val2;
                obj->collider._pos._z = node->val3;
                break;
            case ROTATION_SET:
                obj->collider._rot._x += node->val1;
                obj->collider._rot._y += node->val2;
                obj->collider._rot._z += node->val3;
                obj->collider._rot._w += node->val4;
                break;
            case POSITION_ADD:
                obj->collider._pos._x += node->val1;
                obj->collider._pos._y += node->val2;
                obj->collider._pos._z += node->val3;
                break;
            case ROTATION_ADD:

                break;
            default:
                cout << "ERROR: Invalid change type" << endl;
                break;
        }

        delete node;
    }
}


// Allocate then enqueue
void ObjectBuffer::Enqueue(ObjectChangeNode* node) {
    if(head == NULL) {
        head = node;
        tail = node;
    }
    else {
        tail->next = node;
        tail = node;
    }
}


// Delete after dequeue
ObjectBuffer::ObjectChangeNode* ObjectBuffer::Dequeue() {
    ObjectChangeNode* node = head;
    if(head != NULL) {
        head = head->next;
    }
    return node;
}