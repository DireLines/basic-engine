#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <set>
#include "Component.h"
using namespace std;

class GameObject : Component {
public:
    GameObject();
    ~GameObject();

    int getID();

    set<Component*> getComponents();

    void addComponent(Component* c);

    template<typename C>
    C* getComponent();
private:
    int id;
    set<Component*> components;
};

#endif