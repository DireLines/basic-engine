#include "CollisionSystem.h"
#include "Game.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}

void CollisionSystem::start() {
    for (int i = 0; i < 361; ++i) {
        Point* p = new Point();
        plotpoints.push_back(p);
        Game::instance->instantiate(p);
    }
}

//TODO: complete broad phase to reduce to O(n log n)
void CollisionSystem::update() {
    sort_endpoints();
    vector<Matrix3> matrices(objects.size());
    for (int i = 0; i < objects.size(); ++i) {
        matrices[i] = objects[i]->transform->Apply();
    }
    MinkowskiDifferenceSupport s(objects[0], objects[1]);
    for (int i = 0; i < 360; ++i) {
        Vector2 dir = Transform::Rotate(MathUtils::deg2rad(i)) * Vector2(1, 0);
        Vector2 v = s(dir);
        plotpoints[i]->getComponent<Transform>()->position = v;
    }
    plotpoints[360]->getComponent<Transform>()->position = Vector2(0, 0);
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

bool sameHalfSpace(Vector2 a, Vector2 b) {
    return Vector2::dot(a, b) > 0;
}
bool acute(Vector2 a, Vector2 b, Vector2 c) {
    return Vector2::dot(a - b, c - b) > 0;
}
bool CollisionSystem::GJK_collide(ColliderTransform* a, ColliderTransform* b) {
    MinkowskiDifferenceSupport s(a, b);
    Vector2 origin(0, 0);
    Vector2 p1 = s(Random::unitVector());
    Vector2 p2 = s(-p1);
    Vector2 p3;
    while (true) {
        if (sameHalfSpace(p1, p2)) {
            return false;
        }
        Vector2 diff = p2 - p1;
        Vector2 perp(-diff.y, diff.x);
        if (sameHalfSpace(perp, p2)) {
            perp = -perp;
        }
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