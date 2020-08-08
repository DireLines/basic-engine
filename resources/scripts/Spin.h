#ifndef SPIN_H
#define SPIN_H

#include "Script.h"
#include "Transform.h"

class Spin: public Script {
public:
    double speed = 1;
    Rigidbody* rb;
    void start() {
        rb = gameObject->getComponent<Rigidbody>();
    }
    void update() {
        rb->torque += speed;
    }
    bool parallelizable() {
        return true;
    }
};

#endif