#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <set>
#include "Component.h"
using namespace std;

class GameObject : public Component {
public:
    GameObject();
    ~GameObject();

    int getID();

    set<Component*> getComponents();

    void addComponent(Component* c);

    template<typename C>
    C* getComponent() {
        for (Component* c : components) {
            C* result = dynamic_cast<C*>(c);
            if (result != NULL) {
                return result;
            }
        }
        return NULL;
    }
private:
    int id;
    set<Component*> components;
};

#endif