#ifndef WIGGLE_H
#define WIGGLE_H

#include "Script.h"

class Wiggle: public Script {
public:
    bool parallelizable() {
        return true;
    }
    double strength = 1;
    //TODO: where to put this?
    double random() {
        return (double)rand() / RAND_MAX;
    }
    void start() {
        gameObject->getComponent<Transform>()->position = Vector2(randrange(200, 800), randrange(200, 800));
    }
    void update() {
        Vector2 perturb(strength * randrange(-1, 1), strength * randrange(-1, 1));
        gameObject->getComponent<Transform>()->position += perturb;
        gameObject->getComponent<Transform>()->rotation += randrange(-5, 5);
    }
};

#endif