#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {

}
PhysicsSystem::~PhysicsSystem() {

}
void PhysicsSystem::update() {

}
bool PhysicsSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Rigidbody>() && obj->hasComponent<Transform>();
}
void PhysicsSystem::removeObject(GameObject* obj) {

}
void PhysicsSystem::move(GameObject* obj) {

}
