#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}

//TODO: complete broad phase to reduce to O(n log n)
void CollisionSystem::update() {
    sort_endpoints();
    vector<Matrix3> matrices(objects.size());
    for (int i = 0; i < objects.size(); ++i) {
        matrices[i] = objects[i]->transform->Apply();
    }
    for (int i = 0; i < objects.size(); ++i) {
        ColliderTransform* A = objects[i];
        Matrix3& a_mat = matrices[i];
        Collider* a_col = A->collider;
        for (int j = i + 1; j < objects.size(); ++j) {
            ColliderTransform* B = objects[j];
            Matrix3& b_mat = matrices[j];
            Collider* b_col = B->collider;
            MinkowskiDifferenceSupport s(a_mat, a_col, b_mat, b_col);
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