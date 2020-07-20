#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {

}
CollisionSystem::~CollisionSystem() {

}
void CollisionSystem::update() {

}
bool CollisionSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Collider>() && obj->hasComponent<Transform>();
}
void CollisionSystem::removeObject(GameObject* obj) {

}
void CollisionSystem::addObject(GameObject* obj) {

}

bool CollisionSystem::colliding(GameObject* a, GameObject* b) {
    return false;
}
void CollisionSystem::resolveCollision(GameObject* a, GameObject* b) {

}