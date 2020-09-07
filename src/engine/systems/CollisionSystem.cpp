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
    Transform* t = obj->getComponent<Transform>();

    for (Collider* c : obj->getComponents<Collider>()) {
        ColliderTransform* ct = new ColliderTransform();
        ct->collider = c;
        ct->transform = t;
        objects.push_back(ct);
    }

}
bool gameObjectMatches(ColliderTransform* ct, GameObject* obj) {
    return ct->transform->gameObject == obj;
}
void CollisionSystem::removeObject(GameObject* obj) {
    vector<ColliderTransform*>::iterator it = objects.begin();
    while (it != objects.end()) {
        if ((*it)->transform->gameObject == obj) {
            delete *it;
            it = objects.erase(it);
        } else {
            it++;
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