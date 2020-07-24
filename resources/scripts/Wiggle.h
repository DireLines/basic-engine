#ifndef WIGGLE_H
#define WIGGLE_H

#include "Script.h"
class Wiggle: public Script {
public:
    double strength = 1;

    void update() {
        Vector2 perturb(strength * randrange(-1, 1), strength * randrange(-1, 1));
        gameObject->getComponent<Rigidbody>()->force += perturb;
        // gameObject->getComponent<Transform>()->rotation += randrange(-strength, strength);
    }
    bool parallelizable() {
        return true;
    }
};

#endif