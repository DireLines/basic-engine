#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {
    name = "PhysicsSystem";
}
void PhysicsSystem::update() {
    //TODO: do this in parallel as much as possible
    double dt = GameTimer::DeltaTime();
    for (RigidbodyTransform* obj : objects) {
        move(obj, dt);
    }
}
bool PhysicsSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Rigidbody>() && obj->hasComponent<Transform>();
}

void PhysicsSystem::addObject(GameObject* obj) {
    Rigidbody* rb = obj->getComponent<Rigidbody>();
    Transform* t = obj->getComponent<Transform>();
    RigidbodyTransform* rbt = new RigidbodyTransform();
    rbt->rigidbody = rb;
    rbt->transform = t;
    objects.push_back(rbt);
}

void PhysicsSystem::removeObject(GameObject* obj) {
    for (RigidbodyTransform* rbt : objects) {
        if (rbt->rigidbody->gameObject == obj) {
            VECTOR_ERASE(objects, rbt);
            return;
        }
    }
}
void PhysicsSystem::move(RigidbodyTransform* obj, double dt) {
    Rigidbody* rb = obj->rigidbody;
    Transform* t = obj->transform;

    Vector2 acceleration = Vector2(rb->force.x / rb->mass, rb->force.y / rb->mass);
    rb->velocity.x += acceleration.x * dt;
    rb->velocity.y += acceleration.y * dt;
    t->position.x += rb->velocity.x * dt;
    t->position.y += rb->velocity.y * dt;

    double angular_accel = rb->torque / rb->momentOfInertia;
    rb->angularVelocity += angular_accel * dt;
    t->rotation += rb->angularVelocity * dt;

    //zero out forces
    rb->force = Vector2(0, 0);
    rb->torque = 0;
}