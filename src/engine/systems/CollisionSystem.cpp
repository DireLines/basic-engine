#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}
void CollisionSystem::update() {
    sort_endpoints();
    for (int i = 0; i < objects.size(); ++i) {
        for (int j = i; i < objects.size(); ++i) {
            ColliderTransform* A = objects[i];
            ColliderTransform* B = objects[j];
            MinkowskiDifferenceSupport s(A, B);
            Vector2 dir(1, 0);
            s(dir);
        }
    }
}
bool CollisionSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Collider>() && obj->hasComponent<Transform>();
}
void CollisionSystem::addObject(GameObject* obj) {
    Collider* c = obj->getComponent<Collider>();
    Transform* t = obj->getComponent<Transform>();

    ColliderTransform* ct = new ColliderTransform();
    ct->collider = c;
    ct->transform = t;
    objects.push_back(ct);
}
void CollisionSystem::removeObject(GameObject* obj) {
    for (ColliderTransform* ct : objects) {
        if (ct->transform->gameObject == obj) {
            VECTOR_ERASE(objects, ct);
            return;
        }
    }
}

bool CollisionSystem::GJK_collide(ColliderTransform* a, ColliderTransform* b) {
    Vector2 d1 = Random::unitVector();
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