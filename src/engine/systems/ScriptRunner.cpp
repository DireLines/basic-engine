#include "ScriptRunner.h"

ScriptRunner::ScriptRunner() {}
void ScriptRunner::update() {
    for (Script* script : scripts) {
        if (script->enabled) {
            script->update();
        }
    }
}
void ScriptRunner::addObject(GameObject* obj) {
    for (Script* script : obj->getComponents<Script>()) {
        scripts.insert(script);
        if (script->enabled) {
            script->awake();
            if (script->gameObject->enabled) {
                script->start();
            }
        }
    }
}
void ScriptRunner::removeObject(GameObject* obj) {
    for (Script* script : obj->getComponents<Script>()) {
        scripts.erase(script);
    }
}
bool ScriptRunner::needObject(GameObject* obj) {
    return obj->hasComponent<Script>();
}