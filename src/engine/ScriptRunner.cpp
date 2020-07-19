#include "ScriptRunner.h"
ScriptRunner::ScriptRunner() {}
void ScriptRunner::update() {
    for (GameObject* obj : objects) {
        for (Script* script : obj->getComponents<Script>()) {
            if (script->enabled) {
                script->update();
            }
        }
    }
}
void ScriptRunner::addObject(GameObject* obj) {
    //TODO: instead of having list of GameObjects, have list of Scripts
    objects.insert(obj);
    for (Script* script : obj->getComponents<Script>()) {
        if (script->enabled) {
            script->awake();
            if (obj->enabled) {
                script->start();
            }
        }
    }
}
void ScriptRunner::removeObject(GameObject* obj) {
    objects.erase(obj);
}
