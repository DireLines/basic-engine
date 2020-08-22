#ifndef PRINTCOLLISIONS_H
#define PRINTCOLLISIONS_H

#include "Script.h"

class PrintCollisions: public Script {
public:
    void onCollisionEnter(GameObject* other) {
        cout << gameObject->name << "started colliding with" << other->name << endl;
    }
    void onCollisionStay(GameObject* other) {
        cout << gameObject->name << "colliding with" << other->name << endl;
    }
    void onCollisionExit(GameObject* other) {
        cout << gameObject->name << "stopped colliding with" << other->name << endl;
    }
};

#endif