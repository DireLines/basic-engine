#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "GameObject.h"
#include "System.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Script.h"
#include "Collider.h"
#include <set>
//TODO: store things in a quadtree

using namespace std;

class CollisionSystem : public System {
public:
    CollisionSystem();
    ~CollisionSystem();
    void update();
    bool needObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    set<GameObject*> objects;
    bool colliding(GameObject* a, GameObject* b);
    void resolveCollision(GameObject* a, GameObject* b);
};

#endif