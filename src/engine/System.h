#ifndef SYSTEM_H
#define SYSTEM_H

#include "GameObject.h"

class System {
public:
    System() {}
    ~System() {}
    virtual void update() {}
    virtual bool needObject(GameObject* obj) {return true;}
    void maybeAddObject(GameObject* obj) {
        if (needObject(obj)) {
            addObject(obj);
        }
    }
    virtual void addObject(GameObject* obj) {}
    virtual void removeObject(GameObject* obj) {}
};
#endif