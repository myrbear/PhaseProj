#include "../Headers/PhaseEngine.h"

PhaseEngine::PhaseEngine() {
    engine_running.store(false);
}

// Call to start engine
void PhaseEngine::Run() {
    if(engine_running.load()) {
        cout << "PHASE Engine is already running!" << endl;
    }
    else {
        // Start engine
        engine_running.store(true);
        physics_thread = thread(&PhaseEngine::RunPhysicsThread, this);
        cout << "PHASE Engine Running" << endl;
    }
}


// Call to stop engine
void PhaseEngine::Stop() {
    if(engine_running.load()) {
        // Stop engine
        engine_running.store(false);
        physics_thread.join();
        cout << "PHASE Engine Stopped" << endl;
    }
    else {
        cout << "PHASE Engine is not running" << endl;
    }
}


bool PhaseEngine::IsRunning() {
    return engine_running.load();
}


// This is the thread that performs the physics calculation loop
void PhaseEngine::RunPhysicsThread() {
    auto frame_time = chrono::microseconds(1000000 / FRAME_RATE);
    // auto next_frame = chrono::steady_clock::now();
#ifdef DEBUG
    long i = 0;
    long overshot_frames = 0;
#endif // DEBUG

    // Make windows schedule tasks every 1ms instead of ~15ms
    timeBeginPeriod(1);

    while(engine_running.load()) {
    
        auto loop_start = chrono::steady_clock::now();
        
        // Apply changes to read buffer
        object_buffer.SwapBuffers();
        
        // Run physics calculations
        SimulatePhysics(60.0 / FRAME_RATE);
        
        // Apply external changes
        object_buffer.ApplyChanges();
        
        // Hybrid sleep
        // Sleep for 500us until within 1ms of next frame
        auto next_frame = loop_start + frame_time;
        while(chrono::steady_clock::now() + chrono::milliseconds(1) < next_frame) {
            this_thread::sleep_for(chrono::microseconds(500));
        }

        // Busy wait until next frame
        while(chrono::steady_clock::now() < next_frame) {;}

#ifdef DEBUG
        int duration = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - loop_start).count();
        i++;
        if(duration > frame_time.count()) { overshot_frames++; }
#endif // DEBUG

    }

    timeEndPeriod(1);

#ifdef DEBUG
    cout << "Physics thread complete" << endl;
    cout << "Overshot " << ((float)overshot_frames)/i*100 << "% of frames (" << overshot_frames << "/" << i << ")" << endl;
#endif // DEBUG
}


int PhaseEngine::CreateObject(float side, float mass) {
    int id = object_buffer.CreateObject(side, mass);
    if(id == -1) {
        cout << "ERROR: Object limit reached, cannot create another object." << std::endl;
    }
    return id;
}

int PhaseEngine::CreateStaticObject(float side) {
    int id = object_buffer.CreateStaticObject(side);
    if(id == -1) {
        cout << "ERROR: Object limit reached, cannot create another object." << std::endl;
    }
    return id;
}


GameObject PhaseEngine::GetGameObject(int id) {
    return object_buffer.GetGameObject(id);
}


void PhaseEngine::DeleteObject(int id) {
    object_buffer.DeleteObject(id);
}
void PhaseEngine::SetPosition(int id, float x, float y) {
    object_buffer.SetPosition(id, x, y);
}
void PhaseEngine::SetRotation(int id, float r) {
    object_buffer.SetRotation(id, r);
}
void PhaseEngine::SetVelocity(int id, float vx, float vy) {
    object_buffer.SetVelocity(id, vx, vy);
}
void PhaseEngine::AddPosition(int id, float dx, float dy) {
    object_buffer.AddPosition(id, dx, dy);
}
void PhaseEngine::AddRotation(int id, float dr){
    object_buffer.AddRotation(id, dr);
}
void PhaseEngine::AddVelocity(int id, float dvx, float dvy){
    object_buffer.AddVelocity(id, dvx, dvy);
}


ObjectBuffer::ObjectIterator PhaseEngine::BeginObjIt() {
    return object_buffer.read_begin();
}


ObjectBuffer::ObjectIterator PhaseEngine::EndObjIt() {
    return object_buffer.read_end();
}

ObjectBuffer::ObjectIterator PhaseEngine::BeginPhysIt() {
    return object_buffer.write_begin();
}

ObjectBuffer::ObjectIterator PhaseEngine::EndPhysIt() {
    return object_buffer.write_end();
}


void PhaseEngine::SimulatePhysics(float deltaTime) {
    AccumulateForces(deltaTime);
    IntegrateVelocities(deltaTime);
    CollisionDetection();
    CollisionResolution();
    PositionCorrection();
}


void PhaseEngine::AccumulateForces(float deltaTime) {
    for(auto it = BeginPhysIt(); it != EndPhysIt(); it++) {
        GameObject* obj = *it;
        
        if(!obj->IsStatic()) {
            // Gravity
            obj->velocity.y += deltaTime / 100;
        }
    }
}


void PhaseEngine::IntegrateVelocities(float deltaTime) {
    for(auto it = BeginPhysIt(); it != EndPhysIt(); it++) {
        GameObject* obj = *it;

        if(!obj->IsStatic()) {
            obj->position.x += deltaTime * obj->velocity.x;
            obj->position.y += deltaTime * obj->velocity.y;
            obj->rotation += deltaTime * obj->angular_velocity;
        }
    }
}


void PhaseEngine::CollisionDetection() {
    for(auto ita = BeginPhysIt(); ita != EndPhysIt(); ita++) {
        auto itb = ita;
        itb++;
        while(itb != EndPhysIt()) {
            // Test for collisions
            GameObject* obja = *ita;
            GameObject* objb = *itb;

            if(Collide(obja, objb)) {
                obja->velocity = {0, 0};
                objb->velocity = {0, 0};
            }

            itb++;
        }
    }
}


void PhaseEngine::CollisionResolution() {

}


void PhaseEngine::PositionCorrection() {

}

