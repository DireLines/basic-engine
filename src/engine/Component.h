#ifndef COMPONENT_H
#define COMPONENT_H
class GameObject;

class Component {
public:
    Component() {}
    virtual ~Component() {}
    bool enabled = true;
    GameObject* gameObject = nullptr;
};
#endif