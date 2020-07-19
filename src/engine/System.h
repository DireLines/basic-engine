#ifndef SYSTEM_H
#define SYSTEM_H

#include "GameObject.h"

class System {
public:
    System() {}
    ~System() {}
    virtual void update() {}
    virtual void addObject(GameObject* obj) {}
    virtual void removeObject(GameObject* obj) {}
};
#endif