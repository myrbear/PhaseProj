#include "../Headers/PhaseEngine.h"

PhaseEngine::PhaseEngine() {
    object_buffer = ObjectBuffer();
}

// Call to start engine
void PhaseEngine::Run() {
    running_mtx.lock();
    if(engine_running) {
        cout << "Engine already running!" << endl;
    }
    else {
        // Start engine
        physics_thread = thread(&PhaseEngine::RunPhysicsThread, this);
        cout << "Engine Running" << endl;
        engine_running = true;
    }
    running_mtx.unlock();
}


// Call to stop engine
void PhaseEngine::Stop() {
    running_mtx.lock();
    if(engine_running) {
        // Stop engine
        engine_running = false;
        running_mtx.unlock();
        physics_thread.join();
        cout << "Engine Stopped" << endl;
    }
    else {
        cout << "Engine is not running" << endl;
        running_mtx.unlock();
    }
}


bool PhaseEngine::IsRunning() {
    bool result;
    running_mtx.lock();
    result = engine_running;
    running_mtx.unlock();
    return result;
}


// This is the thread that performs the physics calculation loop
void PhaseEngine::RunPhysicsThread() {
    auto frame_time = chrono::microseconds(1000000 / FRAME_RATE);
    // auto next_frame = chrono::steady_clock::now();
    int i = 0;
    int overshot_frames = 0;

    // Make windows schedule tasks every 1ms instead of ~15ms
    timeBeginPeriod(1);

    while(IsRunning()) {
    
        auto loop_start = chrono::steady_clock::now();

        // Run physics calculations
        SimulatePhysics(frame_time.count());

        // Hybrid sleep
        // Sleep for 500us until within 1ms of next frame
        auto next_frame = loop_start + frame_time;
        while(chrono::steady_clock::now() + chrono::milliseconds(1) < next_frame) {
            this_thread::sleep_for(chrono::microseconds(500));
        }

        // Busy wait until next frame
        while(chrono::steady_clock::now() < next_frame) {;}

        int duration = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - loop_start).count();
        i++;
        if(duration > 17000) { overshot_frames++; }
    }

    timeEndPeriod(1);
}


GameObject* PhaseEngine::CreateObject() {
    // Check buffer capacity
    if(object_buffer.Full()) {
        cout << "ERROR: Object limit reached, cannot create another object." << std::endl;
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