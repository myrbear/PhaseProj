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