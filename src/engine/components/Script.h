#ifndef SCRIPT_H
#define SCRIPT_H

//some engine internals that scripts frequently interact with
#include "GameObject.h"
#include "Vector2.h"
#include "Transform.h"

using namespace std;
class Script: public Component {
public:
    virtual void awake() {}
    virtual void start() {}
    virtual void update() {}

    virtual void onCollisionEnter(GameObject* other) {}
    virtual void onCollisionStay(GameObject* other) {}
    virtual void onCollisionExit(GameObject* other) {}

    //if this returns true, this script promises not to
    //induce race conditions when all instances of
    //the script are run in parallel.
    //EXERCISE CAUTION
    virtual bool parallelizable() {
        return false;
    }
};

#endif