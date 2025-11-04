#include "../Headers/PhaseEngine.h"

PhaseEngine::PhaseEngine() {
    object_buffer = ObjectBuffer();
}


void PhaseEngine::Run() {
    std::cout << "Engine Running" << std::endl;
    
    GameObject* obj1 = CreateObject(0, 0);
    GameObject* obj2 = CreateObject(1, 0);
    GameObject* obj3 = CreateObject(2, 0);

    DeleteObject(obj2);
    
    for(auto it = BeginObjIt(); it != EndObjIt(); it++)
    {
        GameObject* obj_ptr = *it;
        std::cout << "GameObject at (" << obj_ptr->x << "," << obj_ptr->y << ")" << std::endl;
    }
}


GameObject* PhaseEngine::CreateObject(int x, int y) {
    // Check buffer capacity
    if(object_buffer.Full()) {
        std::cout << "ERROR: Object limit reached, cannot create another object." << std::endl;
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