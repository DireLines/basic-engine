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

    int ID();

    //If adding a new public field to GameObject,
    //also add the field's name to the list game_object_fields in codegen.py
    //or .object file parser will assume it's the name of some component.
    //If the field is not intended to be customizable by a .object file,
    //instead add the name to the list ignore_changes
    string name;

    set<Component*> getComponents();

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
            if (result != NULL) {
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
            if (component != NULL) {
                results.insert(component);
            }
        }
        return results;
    }

    GameObject* getChild(int id);
    GameObject* getChild(string name);
    set<GameObject*> getChildren() {
        return getComponents<GameObject>();
    }
    string Type() {
        return type;
    }
protected:
    string type = "GameObject";
private:
    int id;
    set<Component*> components;
    void addComponent(Component* c);
};

#endif