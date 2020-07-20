#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {
    name = "PhysicsSystem";
}
PhysicsSystem::~PhysicsSystem() {

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
    //TODO: update position/velocity
}