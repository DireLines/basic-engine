#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "GameObject.h"
#include "System.h"
#include "Rigidbody.h"
#include "Transform.h"
#include <set>

using namespace std;

struct RigidbodyTransform {
    Rigidbody* rigidbody;
    Transform* transform;
};

class PhysicsSystem : public System {
public:
    PhysicsSystem();
    ~PhysicsSystem();
    void update();
    bool needObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    void addObject(GameObject* obj);
    set<RigidbodyTransform*> objects;
    void move(RigidbodyTransform* obj);
};

#endif