#include "GameObject.h"
#include "IDGenerator.h"

GameObject::GameObject() {
    id = IDGenerator::instance->next();
    name = to_string(id);
}
GameObject::~GameObject() {
    for (Component* c : components) {
        delete c;
    }
}

int GameObject::getID() {
    return id;
}

set<Component*> GameObject::getComponents() {
    return set<Component*>();
}

void GameObject::addComponent(Component* c) {
    components.insert(c);
    c->gameObject = this;
}

GameObject* GameObject::getChild(int id) {
    for (GameObject* obj : getComponents<GameObject>()) {
        if (obj->getID() == id) {
            return obj;
        }
    }
    return NULL;
}

GameObject* GameObject::getChild(string name) {
    for (GameObject* obj : getComponents<GameObject>()) {
        if (obj->name == name) {
            return obj;
        }
    }
    return NULL;
}