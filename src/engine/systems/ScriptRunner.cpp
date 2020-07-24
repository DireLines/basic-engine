#include "ScriptRunner.h"

ScriptRunner::ScriptRunner() {
    name = "ScriptRunner";
}
void ScriptRunner::update() {
    for (Script* script : scripts) {
        if (script->enabled) {
            script->update();
        }
    }
    set<Script*> removedScripts;
    for (Script* script : scripts) {
        if (script->removed) {
            removedScripts.insert(script);
        }
    }
    for (Script* script : removedScripts) {
        scripts.erase(script);
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
        script->enabled = false;
        script->removed = true;
    }
}
bool ScriptRunner::needObject(GameObject* obj) {
    return obj->hasComponent<Script>();
}