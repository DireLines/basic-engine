#include "GameObject.h"
#include "IDGenerator.h"

GameObject::GameObject() {
    id = IDGenerator::instance->next();
}
GameObject::~GameObject() {

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

template<typename C>
C* GameObject::getComponent() {
    for (Component* c : components) {
        C* result = dynamic_cast<C*>(c);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

