#include "../Headers/PhaseEngine.h"

PhaseEngine::PhaseEngine() {
    object_buffer = ObjectBuffer();
}


void PhaseEngine::Run() {
    printf("Engine Running\n");
    
    CreateObject(0, 0);
    CreateObject(1, 0);
    CreateObject(2, 0);
    
    for(auto it = BeginObjIt(); it != EndObjIt(); it++)
    {
        printf("Gameobject at (%i,%i)\n", it->x, it->y);
    }
}


GameObject* PhaseEngine::CreateObject(int x, int y) {
    // Check buffer capacity
    if(object_buffer.Full()) {
        printf("ERROR: Object limit reached, cannot create another object.\n");
        return NULL;
    }
    else {
        // Create Object
        GameObject* new_obj = new GameObject(x, y);
        // Add to buffer
        object_buffer.Add(new_obj);
        return new_obj;
    }
}


void PhaseEngine::DeleteObject(GameObject* ptr) {
    // Remove object from buffer
    if(object_buffer.Remove(ptr)) {
        // Delete object
        delete ptr;
    }
}

ObjectBuffer::ObjectIterator PhaseEngine::BeginObjIt() {
    return object_buffer.begin();
}


ObjectBuffer::ObjectIterator PhaseEngine::EndObjIt() {
    return object_buffer.end();
}