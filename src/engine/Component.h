#ifndef COMPONENT_H
#define COMPONENT_H
using namespace std;
class GameObject;

class Component {
public:
    Component() {}
    virtual ~Component() {}
    bool enabled = true;
    GameObject* gameObject = nullptr;
};
#endif