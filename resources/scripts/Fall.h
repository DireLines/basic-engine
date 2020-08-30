#ifndef FALL_H
#define FALL_H

#include "Script.h"
class Fall: public Script {
public:
    double strength = 9.81;
    Rigidbody* rb;
    void start() {
        rb = gameObject->getComponent<Rigidbody>();
    }
    void update() {
        rb->addForce(Vector2(0, strength));
    }
    bool parallelizable() {
        return true;
    }
};

#endif