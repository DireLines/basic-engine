#ifndef WIGGLE_H
#define WIGGLE_H

#include "Script.h"
class Wiggle: public Script {
public:
    double strength = 1;
    Transform* t;
    Rigidbody* rb;
    void start() {
        t = gameObject->getComponent<Transform>();
        rb = gameObject->getComponent<Rigidbody>();
    }

    void update() {
        Vector2 perturb = Random::insideUnitCircle() * strength;
        t->position += perturb;
        t->rotation += Random::range(-strength, strength);
        rb->velocity += perturb.normalized();
    }
    bool parallelizable() {
        return true;
    }
};

#endif