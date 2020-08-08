#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {
    name = "PhysicsSystem";
}
void PhysicsSystem::update() {
    //TODO: do this in parallel as much as possible
    for (GameObject* obj : objects) {
        move(obj);
    }
}
bool PhysicsSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Rigidbody>() && obj->hasComponent<Transform>();
}

void PhysicsSystem::addObject(GameObject* obj) {
    //TODO: keep track of rigidbody/transform pairs and not GameObjects to avoid dynamic_cast
    objects.insert(obj);
}

void PhysicsSystem::removeObject(GameObject* obj) {
    objects.erase(obj);
}
void PhysicsSystem::move(GameObject* obj) {
    Rigidbody* rb = obj->getComponent<Rigidbody>();
    Transform* t = obj->getComponent<Transform>();
    double dt = 0.016;//TODO: hook time up

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