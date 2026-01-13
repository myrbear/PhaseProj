#include "../Headers/ObjectBuffer.h"

bool ObjectBuffer::Add(GameObject* ptr) {
    if(buffer_index == OBJECT_BUFFER_SIZE) {
        // Object buffer full
        return false;
    }
    else {
        // Insert object pointer
        buffer_array[buffer_index] = ptr;

        // Increment index
        while(buffer_index < OBJECT_BUFFER_SIZE && buffer_array[buffer_index] != NULL) {
            buffer_index++;
        }

        // Object added, return success
        return true;
    }
}


bool ObjectBuffer::Remove(GameObject* ptr) {
    // Find object
    for(int i = 0; i < OBJECT_BUFFER_SIZE; i++) {
        if(buffer_array[i] == ptr) {
            // Object found
            buffer_array[i] = NULL;

            // Update index
            if(i < buffer_index) {
                buffer_index = i;
            }

            // Return success
            return true;
        }
    }

    // Object not found
    return false;
}


bool ObjectBuffer::Full() {
    return buffer_index == OBJECT_BUFFER_SIZE;
}