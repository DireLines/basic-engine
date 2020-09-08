#include "CollisionSystem.h"
#include "Game.h"

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

bool inEdgeRegionAB(Vector2 A, Vector2 B, Vector2 C) {
    Vector2 origin(0, 0);
    if (MathUtils::sameSideOfLine(origin, C, A, B)) {
        if (MathUtils::acute(A, B, origin) && MathUtils::acute(B, A, origin)) {
            return true;
        }
    }
    return false;
}
bool CollisionSystem::GJK_collide(ColliderTransform* a, ColliderTransform* b) {
    MinkowskiDifferenceSupport s(a, b);
    Vector2 origin(0, 0);
    Vector2 p1 = s(Random::unitVector());
    Vector2 p2 = s(-p1);
    Vector2 p3;
    while (true) {
        if (MathUtils::sameHalfSpace(p1, p2)) {
            return false;
        }
        Vector2 perp = MathUtils::perpendicularTowardOrigin(p1, p2);
        p3 = s(perp);
        if (MathUtils::PointInTriangle(origin, p1, p2, p3)) {
            return true;
        }
        //swap points
    }
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