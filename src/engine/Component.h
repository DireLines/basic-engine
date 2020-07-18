#ifndef COMPONENT_H
#define COMPONENT_H
using namespace std;
class Component {
public:
    Component() {}
    virtual ~Component() {}
    bool enabled = true;
};
#endif