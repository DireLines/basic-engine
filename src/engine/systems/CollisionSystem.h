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

struct IntervalEndpoint {
    double pos;
    bool begin;
    Collider* collider;
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