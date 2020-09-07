#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "GameObject.h"
#include "System.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Script.h"
#include "Collider.h"
#include "Vector2.h"
#include <vector>
#include "Random.h"

using namespace std;

struct TransformCollider {
    Transform* transform;
    Collider* collider;
};

struct IntervalEndpoint {
    double pos;
    bool begin;
    TransformCollider* tc;
};

class MinkowskiDifferenceSupport {
public:
    MinkowskiDifferenceSupport(TransformCollider* A, TransformCollider* B) {
        A_transform = A->transform;
        A_collider = A->collider;
        B_transform = B->transform;
        B_collider = B->collider;
    }
    Vector2 operator()(Vector2 direction) {
        return transformedSupport(direction, A_transform, A_collider) - transformedSupport(direction, B_transform, B_collider);
    }
private:
    Transform* A_transform;
    Collider* A_collider;
    Transform* B_transform;
    Collider* B_collider;
    Vector2 transformedSupport(Vector2 direction, Transform* transform, Collider* collider) {
        Matrix3 t = transform->Apply();
        double rotation = Vector2::calculateRotation(Vector2(0, 0), t * Vector2(1, 0));
        Vector2 orig_support = collider->support(Transform::Rotate(-rotation) * direction);
        return t * orig_support;
    }
};

class CollisionSystem : public System {
public:
    CollisionSystem();
    ~CollisionSystem();
    void update();
    bool needObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    vector<IntervalEndpoint*> endpoints;
    void addObject(GameObject* obj);
    bool GJK_collide(Collider* a, Collider* b);
    bool colliding(GameObject* a, GameObject* b);
    void resolveCollision(GameObject* a, GameObject* b);
    void sort_endpoints();
};

#endif