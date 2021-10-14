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
#include <thread>
#include <future>
using namespace std;

struct ColliderMatrices {
    Matrix3 applied_transform;
    Matrix3 undo_rotation;
    Collider* collider;
};

struct ColliderTransform {
    Collider* collider;
    Transform* transform;
};

struct Collision {
    Collider* a;
    Collider* b;
};
struct Interval {
    double begin;//position of interval's beginning
    double end;//position of interval's end
    ColliderTransform* object;
    ColliderMatrices precalculated;
};

class MinkowskiDifferenceSupport {
public:
    MinkowskiDifferenceSupport(ColliderTransform* A, ColliderTransform* B);
    MinkowskiDifferenceSupport(ColliderMatrices& A, ColliderMatrices& B);
    ~MinkowskiDifferenceSupport();

    Vector2 operator()(Vector2 direction);
    static Vector2 transformedSupport(Vector2 direction, Matrix3& t, Collider* collider);
    static Vector2 transformedSupport(Vector2 direction, Matrix3& t, Collider* collider, Matrix3& rotation);
private:
    Matrix3 A_transform;
    Collider* A_collider;
    Matrix3 A_rotation;
    Matrix3 B_transform;
    Collider* B_collider;
    Matrix3 B_rotation;
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
    vector<Interval> intervals;
    void addObject(GameObject* obj);
    bool GJK_collide(ColliderMatrices a, ColliderMatrices b);
    bool colliding(GameObject* a, GameObject* b);
    void resolveCollision(GameObject* a, GameObject* b);
    void update_endpoint_positions();
    void sort_intervals();
    void precalculate_matrices();
    void detect_collisions(int thread_id, vector<Collision>& output);
};

#endif