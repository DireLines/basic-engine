#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {
    name = "PhysicsSystem";
}
void PhysicsSystem::update() {
    double dt = GameTimer::DeltaTime();
    for (RigidbodyTransform obj : objects) {
        move(obj, dt);
    }
}
bool PhysicsSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Rigidbody>() && obj->hasComponent<Transform>();
}

void PhysicsSystem::addObject(GameObject* obj) {
    objects.push_back(RigidbodyTransform{
        .rigidbody = obj->getComponent<Rigidbody>(),
        .transform = obj->getComponent<Transform>()
    });
}

void PhysicsSystem::removeObject(GameObject* obj) {
    vector<RigidbodyTransform>::iterator it = objects.begin();
    while (it != objects.end()) {
        if ((*it).transform->gameObject == obj) {
            it = objects.erase(it);
        } else {
            it++;
        }
    }
}
void PhysicsSystem::move(RigidbodyTransform obj, double dt) {
    Rigidbody* rb = obj.rigidbody;
    Transform* t = obj.transform;

    Vector2 acceleration = Vector2(rb->force.x / rb->mass, rb->force.y / rb->mass);
    rb->velocity += acceleration * dt;
    t->position += rb->velocity * dt;

    double angular_accel = rb->torque / rb->momentOfInertia;
    rb->angularVelocity += angular_accel * dt;
    t->rotation += rb->angularVelocity * dt;

    //zero out forces
    rb->force = Vector2(0, 0);
    rb->torque = 0;
}