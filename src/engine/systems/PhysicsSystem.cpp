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
    objects.insert(obj);
}

void PhysicsSystem::removeObject(GameObject* obj) {
    objects.erase(obj);
}
void PhysicsSystem::move(GameObject* obj) {
    Rigidbody* rb = obj->getComponent<Rigidbody>();
    Transform* t = obj->getComponent<Transform>();
    //TODO: update position/velocity
    //zero out forces
    rb->force = Vector2(0, 0);
    rb->torque = 0;
}