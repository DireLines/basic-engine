#ifndef SYSTEM_H
#define SYSTEM_H

#include "GameObject.h"

class System {
public:
    System() {}
    ~System() {}
    virtual void start() {}
    virtual void update() {}
    virtual bool needObject(GameObject* obj) {return true;}
    void maybeAddObject(GameObject* obj) {
        if (needObject(obj)) {
            addObject(obj);
        }
    }
    virtual void removeObject(GameObject* obj) {}
    string getName() {return name;}
protected:
    virtual void addObject(GameObject* obj) {}
    string name = "system"; //for debugging
};
#endif