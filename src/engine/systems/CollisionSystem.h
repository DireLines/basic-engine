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
#include "GameObjects.h"

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
    MinkowskiDifferenceSupport(ColliderTransform* A, ColliderTransform* B);
    MinkowskiDifferenceSupport(Matrix3& a_mat, Collider* a_col, Matrix3& b_mat, Collider* b_col);
    ~MinkowskiDifferenceSupport();

    Vector2 operator()(Vector2 direction);
    //actually do the minkowski sum, because it's easier to check correctness
    Vector2 sum(Vector2 direction);
    static Vector2 transformedSupport(Vector2 direction, Matrix3& t, Collider* collider);
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
    void start();
    void update();
    bool needObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    vector<IntervalEndpoint*> endpoints;
    vector<ColliderTransform*> objects;
    vector<Point*> plotpoints;
    void addObject(GameObject* obj);
    bool GJK_collide(ColliderTransform* a, ColliderTransform* b);
    bool colliding(GameObject* a, GameObject* b);
    void resolveCollision(GameObject* a, GameObject* b);
    void sort_endpoints();
};

#endif