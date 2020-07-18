#include "GameObject.h"
#include "IDGenerator.h"

GameObject::GameObject() {
    id = IDGenerator::instance->next();
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
}