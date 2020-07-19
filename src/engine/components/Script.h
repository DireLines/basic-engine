#ifndef SCRIPT_H
#define SCRIPT_H

#include "Component.h"
using namespace std;
class Script: public Component {
public:
    virtual void awake() {}
    virtual void start() {}
    virtual void update() {}
    //if set to true, this script promises not to
    //induce race conditions when run in parallel
    const bool parallelizable = false;
};

#endif