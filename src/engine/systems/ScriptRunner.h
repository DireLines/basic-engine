#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include "GameObject.h"
#include "Script.h"
#include "System.h"
#include <set>

using namespace std;

class ScriptRunner : public System {
public:
    ScriptRunner();
    ~ScriptRunner();
    void update();
    bool needObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    void addObject(GameObject* obj);
    set<Script*> scripts;
};

#endif