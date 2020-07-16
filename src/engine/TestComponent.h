#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H
#include "Component.h"
using namespace std;
class TestComponent: public Component {
public:
    TestComponent(int v) {
        this->value = v;
    }
    ~TestComponent() {}
    int value;
};
#endif