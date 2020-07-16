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

vector<Component*> GameObject::getComponents() {
    return vector<Component*>();
}

void GameObject::addComponent(Component* c) {

}

template<class C>
C* GameObject::getComponent() {
    return new C();
}

