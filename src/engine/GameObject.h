#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <set>
#include <string>
#include "Component.h"
using namespace std;

class GameObject : public Component {
public:
    GameObject();
    ~GameObject();

    int getID();
    string name;

    set<Component*> getComponents();

    void addComponent(Component* c);

    template<typename C>
    C* addComponent() {
        C* c = new C();
        addComponent(c);
        return c;
    }

    template<typename C>
    bool hasComponent() {
        return getComponent<C>() != NULL;
    }

    template<typename C>
    C* getComponent() {
        for (Component* c : components) {
            C* result = dynamic_cast<C*>(c);
            if (result != NULL && result->enabled) {
                return result;
            }
        }
        return NULL;
    }

    template<typename C>
    set<C*> getComponents() {
        set<C*> results;
        for (Component* c : components) {
            C* component = dynamic_cast<C*>(c);
            if (component != NULL && component->enabled) {
                results.insert(component);
            }
        }
        return results;
    }

    GameObject* getChild(int id);
    GameObject* getChild(string name);
private:
    int id;
    set<Component*> components;
};

#endif