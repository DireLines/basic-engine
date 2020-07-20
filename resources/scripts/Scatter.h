#ifndef SCATTER_H
#define SCATTER_H

#include "Script.h"
class Scatter: public Script {
public:
    double range = 500;
    double rotationRange = 180;
    void start() {
        Transform* t = gameObject->getComponent<Transform>();
        t->rotation = randrange(-rotationRange, rotationRange);
        t->position = Vector2(randrange(-range, range), randrange(-range, range));
    }
    bool parallelizable() {
        return true;
    }
};

#endif