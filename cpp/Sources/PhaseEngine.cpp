#include "../Headers/PhaseEngine.h"

PhaseEngine::PhaseEngine() {
    engine_running.store(false);
    // sensible defaults
    gravity = 9.8f * 100.0f; // scale to pixels/sec^2
    friction = 0.5f; // arbitrary damping
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

void PhaseEngine::SetGravity(float g) {
    gravity = g;
}

void PhaseEngine::SetFriction(float f) {
    friction = f;
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


int PhaseEngine::CreateObject(float side, float mass, int color) {
    int id = object_buffer.CreateObject(side, mass, color);
    if(id == -1) {
        cout << "ERROR: Object limit reached, cannot create another object." << std::endl;
    }
    return id;
}

int PhaseEngine::CreateStaticObject(float side, int color) {
    int id = object_buffer.CreateStaticObject(side, color);
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
    CollisionResolution();
}


void PhaseEngine::AccumulateForces(float deltaTime) {
    for(auto it = BeginPhysIt(); it != EndPhysIt(); it++) {
        GameObject* obj = *it;
        
        if(!obj->IsStatic()) {
                // Gravity (scaled by engine gravity)
                obj->velocity.y += gravity * deltaTime;
                // simple damping
                float damp = 1.0f - friction * deltaTime;
                if(damp < 0.0f) damp = 0.0f;
                obj->velocity.x *= damp;
                obj->velocity.y *= damp;
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


void PhaseEngine::CollisionResolution() {
    for(auto ita = BeginPhysIt(); ita != EndPhysIt(); ita++) {
        auto itb = ita;
        itb++;
        while(itb != EndPhysIt()) {
            // Test for collisions
            GameObject* obja = *ita;
            GameObject* objb = *itb;

            // Static objects don't collide with each other
            if(obja->IsStatic() && objb->IsStatic()) {
                itb++;
                continue;
            }

            CollisionInfo info;

            if(Collide(obja, objb, &info)) {
                // Relative velocity
                Vector v_rel = objb->velocity - obja->velocity;
                float v_n = Dot(v_rel, info.contact_normal);
                if(v_n <= 0) {
                    // Bodies colliding, impulse needed
                    Vector ra = info.contact_point - obja->position;
                    Vector rb = info.contact_point - objb->position;
                    float ra_n = Cross(ra, info.contact_normal);
                    float rb_n = Cross(rb, info.contact_normal);
                    float k = obja->inv_mass + objb->inv_mass + (ra_n*ra_n) * obja->inv_inertia + (rb_n*rb_n) * objb->inv_inertia;  // Effective mass
                    float e = 0.2;
                    float j = -(1 + e) * v_n / k; // Impulse magnitude
                    Vector J = info.contact_normal * j;

                    // Linear impulse
                    obja->velocity = obja->velocity - (J * obja->inv_mass);
                    objb->velocity = objb->velocity + (J * objb->inv_mass);

                    // Angular impulse
                    obja->angular_velocity = obja->angular_velocity - (Cross(ra, J) * obja->inv_inertia);
                    objb->angular_velocity = objb->angular_velocity + (Cross(rb, J) * objb->inv_inertia);

                    // Position correction
                    float mass_factor = obja->inv_mass / (obja->inv_mass + objb->inv_mass);
                    obja->position = obja->position - (info.contact_normal * mass_factor * info.penetration_depth);
                    objb->position = objb->position + (info.contact_normal * (1 - mass_factor) * info.penetration_depth);
                }
            }

            itb++;
        }
    }
}

