#ifndef PHASE_ENGINE
#define PHASE_ENGINE

#include "stdio.h"
#include <iostream>
#include "../Headers/GameObject.h"
#include "../Headers/ObjectBuffer.h"

class PhaseEngine {
    public:
        PhaseEngine();
        void Run();
        GameObject* CreateObject(int x, int y);
        void DeleteObject(GameObject* ptr);
        ObjectBuffer::ObjectIterator BeginObjIt();
        ObjectBuffer::ObjectIterator EndObjIt();

    private:
        ObjectBuffer object_buffer;
};

#endif