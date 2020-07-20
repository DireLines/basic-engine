#ifndef SPIN_H
#define SPIN_H

#include "Script.h"
#include "Transform.h"

class Spin: public Script {
public:
    bool parallelizable() {
        return true;
    }
    double speed;
    void update();
};

#endif