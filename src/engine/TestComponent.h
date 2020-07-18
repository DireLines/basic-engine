#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H
#include "Component.h"
#include <string>
using namespace std;
class TestComponent: public Component {
public:
    TestComponent() {
        value = 0;
        name = "default";
    }
    int value;
    string name;
};
#endif