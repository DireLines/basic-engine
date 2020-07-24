#ifndef SPIN_H
#define SPIN_H

#include "Script.h"
#include "Transform.h"

class Spin: public Script {
public:
    double speed = 1;
    void update();
    bool parallelizable() {
        return true;
    }
};

#endif