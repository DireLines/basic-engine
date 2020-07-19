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
    void addObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    set<Script*> scripts;
};

#endif