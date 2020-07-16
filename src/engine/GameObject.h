#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Component.h"
using namespace std;

class GameObject : Component {
public:
    GameObject();
    ~GameObject();

    int getID();

    vector<Component*> getComponents();

    void addComponent(Component* c);

    template<class C>
    C* getComponent();
private:
    int id;
    vector<Component*> components;
};

#endif