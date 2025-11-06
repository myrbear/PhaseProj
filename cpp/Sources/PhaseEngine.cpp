#include "../Headers/PhaseEngine.h"

PhaseEngine::PhaseEngine() {
    object_buffer = ObjectBuffer();
}


void PhaseEngine::Run() {
    std::cout << "Engine Running" << std::endl;
    
}


GameObject* PhaseEngine::CreateObject() {
    // Check buffer capacity
    if(object_buffer.Full()) {
        std::cout << "ERROR: Object limit reached, cannot create another object." << std::endl;
        return NULL;
    }
    else {
        // Create Object
        GameObject* new_obj = new GameObject();
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

ObjectBuffer::ObjectIterator PhaseEngine::BeginPhysIt() {
    return object_buffer.begin();
}

ObjectBuffer::ObjectIterator PhaseEngine::EndPhysIt() {
    return object_buffer.end();
}


void PhaseEngine::SimulatePhysics(float deltaTime) {
    // Example iterator loop
    for(auto it = BeginPhysIt(); it != EndPhysIt(); it++) {
        // Dereference iterator for object pointer
        GameObject* gameObject = *it;
    }
}