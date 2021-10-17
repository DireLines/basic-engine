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
#include <tuple>
#include <GameObjects.h>
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
    ColliderTransform a;
    ColliderTransform b;
};
struct Interval {
    double begin;//position of interval's beginning
    double end;//position of interval's end
    ColliderTransform object;
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
    vector<Point*> pointObjects;
    void addObject(GameObject* obj);
    Vector2 EPA_closestPoint(MinkowskiDifferenceSupport s, Vector2 p1, Vector2 p2, Vector2 p3);
    bool GJK_collide(ColliderMatrices a, ColliderMatrices b);
    bool colliding(ColliderTransform& a, ColliderTransform& b);
    void resolveCollision(ColliderTransform& a, ColliderTransform& b);
    void update_endpoint_positions();
    void sort_intervals();
    void precalculate_matrices();
    vector<Collision> detectCollisions();
    void detect_collisions_thread(int thread_id, vector<Collision>& output);
    void resolveCollisions(vector<Collision> &collisions);
    ColliderMatrices precalculate(ColliderTransform ct);
};

#endif