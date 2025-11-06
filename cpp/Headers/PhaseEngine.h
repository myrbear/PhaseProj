#ifndef PHASE_ENGINE
#define PHASE_ENGINE

#include <iostream>
#include <thread>
#include <mutex>
#include "../Headers/GameObject.h"
#include "../Headers/ObjectBuffer.h"

using namespace std;

class PhaseEngine {
    public:
        PhaseEngine();
        void Run();
        void Stop();
        GameObject* CreateObject();
        void DeleteObject(GameObject* ptr);
        // For external use (ALEX)
        ObjectBuffer::ObjectIterator BeginObjIt();
        ObjectBuffer::ObjectIterator EndObjIt();

        bool IsRunning();

    private:
        // Main phyisics loop
        void SimulatePhysics(float deltaTime);
        // For physics calculations (MYRON)
        ObjectBuffer::ObjectIterator BeginPhysIt();
        ObjectBuffer::ObjectIterator EndPhysIt();

        ObjectBuffer object_buffer;

        // Threading
        void RunPhysicsThread();
        thread physics_thread;
        bool engine_running = false;
        mutex running_mtx;
};

#endif