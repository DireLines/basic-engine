#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "GameObject.h"
#include "System.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Script.h"
#include "Collider.h"
#include "Vector2.h"
#include <set>

using namespace std;

struct AxisAlignedBoundingBox {
    Vector2 min; //corner with smallest x and y vals
    Vector2 max; //corner with largest x and y vals
};

struct BoundingBoxEndpoint {
    bool begin;
    AxisAlignedBoundingBox* box;
};

class CollisionSystem : public System {
public:
    CollisionSystem();
    ~CollisionSystem();
    void update();
    bool needObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    vector<GameObject*> objects;
    void addObject(GameObject* obj);
    bool colliding(GameObject* a, GameObject* b);
    void resolveCollision(GameObject* a, GameObject* b);
    void sort_intervals();
};

#endif