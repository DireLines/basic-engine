#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H
#include "Component.h"
using namespace std;
class TestComponent: public Component {
public:
    TestComponent() {
        value = 0;
    }
    // ~TestComponent() {}
    int value;
};
#endif