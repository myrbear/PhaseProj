#ifndef PHASE_ENGINE
#define PHASE_ENGINE

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <cmath>
#include "../Headers/GameObject.h"
#include "../Headers/ObjectBuffer.h"

// Windows
#undef byte
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#define NOUSER
#include <windows.h>
#include <mmsystem.h>

#define FRAME_RATE 60

#define DEBUG

using namespace std;

class PhaseEngine {
    public:
        PhaseEngine();
        void Run();
        void Stop();
        int CreateObject(float side, float mass, int color);
        int CreateStaticObject(float side, int color);
        void DeleteObject(int id);
        // Object read functions
        // For external use (ALEX)
        ObjectBuffer::ObjectIterator BeginObjIt();
        ObjectBuffer::ObjectIterator EndObjIt();
        GameObject GetGameObject(int id);

        // Object write functions
        void SetPosition(int id, float x, float y);
        void SetRotation(int id, float r);
        void SetVelocity(int id, float vx, float vy);
        void AddPosition(int id, float dx, float dy);
        void AddRotation(int id, float dr);
        void AddVelocity(int id, float dvx, float dvy);

        bool IsRunning();

    private:
        // Main phyisics loop
        void SimulatePhysics(float deltaTime);
        // Physics stages
        void AccumulateForces(float deltaTime);
        void IntegrateVelocities(float deltaTime);
        void CollisionResolution();

        // For physics calculations (MYRON)
        ObjectBuffer::ObjectIterator BeginPhysIt();
        ObjectBuffer::ObjectIterator EndPhysIt();

        ObjectBuffer object_buffer;

        // Threading
        void RunPhysicsThread();
        thread physics_thread;
        atomic<bool> engine_running;
};

#endif