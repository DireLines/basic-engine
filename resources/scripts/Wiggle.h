#ifndef WIGGLE_H
#define WIGGLE_H

#include "Script.h"

using namespace std;
class Wiggle: public Script {
public:
    bool parallelizable() {
        return true;
    }
    double strength = 1;
    void update() {
        Vector2 perturb(strength * random(), strength * random());
        gameObject->getComponent<Transform>()->position += perturb;
    }
};

#endif