#ifndef SCATTER_H
#define SCATTER_H

#include "Script.h"
class Scatter: public Script {
public:
    double range = 300;
    double rotationRange = 180;
    double maxSpeed = 50;
    void start() {
        Transform* t = gameObject->getComponent<Transform>();
        t->rotation = Random::range(-rotationRange, rotationRange);
        t->position = Random::insideUnitCircle() * range;
        Rigidbody* rb = gameObject->getComponent<Rigidbody>();
        if(rb) {
            rb->velocity = Random::insideUnitCircle() * maxSpeed;
            rb->angularVelocity = Random::range(-maxSpeed, maxSpeed);
        }
    }
    bool parallelizable() {
        return true;
    }
};

#endif