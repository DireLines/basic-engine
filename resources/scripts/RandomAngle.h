#ifndef RANDOMANGLE_H
#define RANDOMANGLE_H

#include "Script.h"
class RandomAngle: public Script {
public:
    void start() {
        Transform* t = gameObject->getComponent<Transform>();
        t->rotation = randrange(-180, 180);
    }
};

#endif