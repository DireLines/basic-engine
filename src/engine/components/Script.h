#ifndef SCRIPT_H
#define SCRIPT_H

//some engine internals that scripts frequently interact with
#include "GameObject.h"
#include "Vector2.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "GameTimer.h"
#include "Input.h"
#include "Random.h"

using namespace std;
class Script: public Component {
protected:
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

    bool removed = false;//marker for ScriptRunner to delete this script at end of update
    friend class ScriptRunner;
    friend class CollisionSystem;
};

#endif