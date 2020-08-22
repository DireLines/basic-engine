#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}
void CollisionSystem::update() {
    sort_endpoints();
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

bool pos(IntervalEndpoint* a, IntervalEndpoint* b) {
    return a->pos < b->pos;
}

void CollisionSystem::sort_endpoints() {
    std::sort(endpoints.begin(), endpoints.end(), pos);
}