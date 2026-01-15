#include "../Headers/ObjectBuffer.h"


ObjectBuffer::ObjectBuffer() {
    read_buffer_ptr = bufferA;
    write_buffer_ptr = bufferB;
}


int ObjectBuffer::CreateObject(float side, float mass, int color) {
    if(buffer_index == OBJECT_BUFFER_SIZE) {
        // Object buffer full, return invalid Id
        return -1;
    }
    else {
        // New Id is buffer index
        int new_id = buffer_index;

        // Enqueue update
        ObjectChangeNode* node = new ObjectChangeNode(OBJECT_CREATE, new_id, side, mass, 0, color);
        Enqueue(node);

        // Increment index
        buffer_index++;
        while(buffer_index < OBJECT_BUFFER_SIZE && read_buffer_ptr[buffer_index] != NULL) {
            buffer_index++;
        }

        // Object will be added, return id
        return new_id;
    }
}


int ObjectBuffer::CreateStaticObject(float side, int color) {
    if(buffer_index == OBJECT_BUFFER_SIZE) {
        // Object buffer full, return invalid Id
        return -1;
    }
    else {
        // New Id is buffer index
        int new_id = buffer_index;

        // Enqueue update
        ObjectChangeNode* node = new ObjectChangeNode(OBJECT_CREATE, new_id, side, 0, 1, color);
        Enqueue(node);

        // Increment index
        buffer_index++;
        while(buffer_index < OBJECT_BUFFER_SIZE && read_buffer_ptr[buffer_index] != NULL) {
            buffer_index++;
        }

        // Object will be added, return id
        return new_id;
    }
}


void ObjectBuffer::DeleteObject(int id) {
    // Validate Id
    if(id < 0 || id >= OBJECT_BUFFER_SIZE) {
        cout << "ERROR: Invalid Object Id" << endl;
        return;
    }
    // Enqueue update
    ObjectChangeNode* node = new ObjectChangeNode(OBJECT_DELETE, id);
    Enqueue(node);
    // Update buffer index
    if(buffer_index > id) {
        buffer_index = id;
    }
}


GameObject ObjectBuffer::GetGameObject(int id) {
    shared_lock lock(mtx);
    // Validate Id
    if(id < 0 || id >= OBJECT_BUFFER_SIZE) {
        cout << "ERROR: Invalid Object Id" << endl;
        return GameObject(-1, 0, 0, false, 0);
    }

    // Retrieve object
    GameObject* ptr = read_buffer_ptr[id];
    if(ptr == NULL) {
        cout << "ERROR: Invalid Object Id" << endl;
        return GameObject(-1, 0, 0, false, 0);
    }
    else {
        return *ptr;
    }

}


bool ObjectBuffer::Full() {
    return buffer_index == OBJECT_BUFFER_SIZE;
}


void ObjectBuffer::SetPosition(int id, float x, float y) {
    // Enqueue change
    ObjectChangeNode* node = new ObjectChangeNode(POSITION_SET, id, x, y);
    Enqueue(node);
}

void ObjectBuffer::SetRotation(int id, float r) {
    ObjectChangeNode* node = new ObjectChangeNode(ROTATION_SET, id, r);
    Enqueue(node);
}

void ObjectBuffer::SetVelocity(int id, float vx, float vy) {
    ObjectChangeNode* node = new ObjectChangeNode(VELOCITY_SET, id, vx, vy);
    Enqueue(node);
}

void ObjectBuffer::AddPosition(int id, float dx, float dy) {
    ObjectChangeNode* node = new ObjectChangeNode(POSITION_ADD, id, dx, dy);
    Enqueue(node);
}


void ObjectBuffer::AddRotation(int id, float dr) {
    ObjectChangeNode* node = new ObjectChangeNode(ROTATION_ADD, id, dr);
    Enqueue(node);
}

void ObjectBuffer::AddVelocity(int id, float dvx, float dvy) {
    ObjectChangeNode* node = new ObjectChangeNode(VELOCITY_ADD, id, dvx, dvy);
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
    unique_lock lock(queue_mtx);

    // Dequeue until empty
    while(head != NULL) {
        ObjectChangeNode* node = Dequeue();

        // Check if creation is needed
        if(node->change_type == OBJECT_CREATE) {
            // Allocate memory for two objects
            bool is_static = (node->val3 == 1);
            GameObject* read_obj = new GameObject(node->id, node->val1, node->val2, is_static, node->col);
            GameObject* write_obj = new GameObject(node->id, node->val1, node->val2, is_static, node->col);

            // Insert object pointer
            read_buffer_ptr[node->id] = read_obj;
            write_buffer_ptr[node->id] = write_obj; 

            delete node;
            continue;
        }

        // Check Id validity
        if(node->id < 0 || node->id >= OBJECT_BUFFER_SIZE || write_buffer_ptr[node->id] == NULL) {
            cout << "ERROR: invalid GameObject Id" << endl;
            delete node;
            continue;
        }

        GameObject* obj = write_buffer_ptr[node->id];
        GameObject* read_obj = read_buffer_ptr[node->id];

        switch(node->change_type) {
            case OBJECT_DELETE:
                // Delete both copies
                delete read_obj;
                delete obj;
                // Remove pointers
                read_buffer_ptr[node->id] = NULL;
                write_buffer_ptr[node->id] = NULL;
                break;
            case POSITION_SET:
                obj->position.x = node->val1;
                obj->position.y = node->val2;
                break;
            case ROTATION_SET:
                obj->rotation += node->val1;
                break;
            case VELOCITY_SET:
                obj->velocity.x = node->val1;
                obj->velocity.y = node->val2;
                break;
            case POSITION_ADD:
                obj->position.x += node->val1;
                obj->position.y += node->val2;
                break;
            case ROTATION_ADD:
                obj->rotation += node->val1;
                break;
            case VELOCITY_ADD:
                obj->velocity.x += node->val1;
                obj->velocity.y += node->val2;
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
    unique_lock lock(queue_mtx);
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
// Mutex locked in ApplyChanges()
ObjectBuffer::ObjectChangeNode* ObjectBuffer::Dequeue() {
    ObjectChangeNode* node = head;
    if(head != NULL) {
        head = head->next;
    }
    return node;
}