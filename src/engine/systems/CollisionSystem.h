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

struct ColliderTransform {
    Collider* collider;
    Transform* transform;
};

struct IntervalEndpoint {
    double pos;
    bool begin;
    ColliderTransform* ct;
};

class MinkowskiDifferenceSupport {
public:
    MinkowskiDifferenceSupport(ColliderTransform* A, ColliderTransform* B) {
        A_transform = A->transform->Apply();
        A_collider = A->collider;
        B_transform = B->transform->Apply();
        B_collider = B->collider;
    }
    MinkowskiDifferenceSupport(Matrix3& a_mat, Collider* a_col, Matrix3& b_mat, Collider* b_col) {
        A_transform = a_mat;
        A_collider = a_col;
        B_transform = b_mat;
        B_collider = b_col;
    }
    Vector2 operator()(Vector2 direction) {
        return transformedSupport(direction, A_transform, A_collider) - transformedSupport(direction, B_transform, B_collider);
    }
    //actually do the minkowski sum, because it's easier to check correctness
    Vector2 sum(Vector2 direction) {
        return transformedSupport(direction, A_transform, A_collider) + transformedSupport(direction, B_transform, B_collider);
    }
    static Vector2 transformedSupport(Vector2 direction, Matrix3& t, Collider* collider) {
        double rotation = Vector2::calculateRotation(Vector2(0, 0), t * Vector2(1, 0) - t * Vector2(0, 0));
        Vector2 orig_support = collider->support(Transform::Rotate(-rotation) * direction);
        return t * orig_support;
    }
private:
    Matrix3 A_transform;
    Collider* A_collider;
    Matrix3 B_transform;
    Collider* B_collider;
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
    vector<ColliderTransform*> objects;
    void addObject(GameObject* obj);
    bool GJK_collide(ColliderTransform* a, ColliderTransform* b);
    bool colliding(GameObject* a, GameObject* b);
    void resolveCollision(GameObject* a, GameObject* b);
    void sort_endpoints();
};

#endif