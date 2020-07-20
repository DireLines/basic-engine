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
