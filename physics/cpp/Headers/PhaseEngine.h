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
        GameObject* CreateObject();
        void DeleteObject(GameObject* ptr);
        // For external use (ALEX)
        ObjectBuffer::ObjectIterator BeginObjIt();
        ObjectBuffer::ObjectIterator EndObjIt();

    private:
        // Main phyisics loop
        void SimulatePhysics(float deltaTime);
        // For physics calculations (MYRON)
        ObjectBuffer::ObjectIterator BeginPhysIt();
        ObjectBuffer::ObjectIterator EndPhysIt();

        ObjectBuffer object_buffer;
};

#endif