#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}
void CollisionSystem::update() {

}
bool CollisionSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Collider>() && obj->hasComponent<Transform>();
}
void CollisionSystem::removeObject(GameObject* obj) {
    VECTOR_ERASE(objects, obj);
}
void CollisionSystem::addObject(GameObject* obj) {
    objects.push_back(obj);
}

bool CollisionSystem::colliding(GameObject* a, GameObject* b) {
    return false;
}
void CollisionSystem::resolveCollision(GameObject* a, GameObject* b) {
}