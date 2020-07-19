#ifndef SCRIPT_H
#define SCRIPT_H

#include "Component.h"
using namespace std;
class Script: public Component {
public:
    virtual void awake() {}
    virtual void start() {}
    virtual void update() {}

    //if this returns true, this script promises not to
    //induce race conditions when run in parallel
    virtual bool parallelizable() {
        return false;
    }
};

#endif