#ifndef SPIN_H
#define SPIN_H

#include "Script.h"
#include "Transform.h"

using namespace std;
class Spin: public Script {
public:
    const bool parallelizable = true;
    double speed;
    void update();
};

#endif