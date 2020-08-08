#ifndef WIGGLE_H
#define WIGGLE_H

#include "Script.h"
class Wiggle: public Script {
public:
    double strength = 1;
    Transform* t;

    void start() {
        t = gameObject->getComponent<Transform>();
    }

    void update() {
        Vector2 perturb(strength * randrange(-1, 1), strength * randrange(-1, 1));
        t->position += perturb;
        t->rotation += randrange(-strength, strength);
    }
    bool parallelizable() {
        return true;
    }
};

#endif