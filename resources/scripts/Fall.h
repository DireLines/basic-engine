#ifndef FALL_H
#define FALL_H

#include "Script.h"
class Fall: public Script {
public:
    double strength = 9.81;
    void update() {
        gameObject->getComponent<Rigidbody>()->force += Vector2(0, strength);
    }
    bool parallelizable() {
        return true;
    }
};

#endif