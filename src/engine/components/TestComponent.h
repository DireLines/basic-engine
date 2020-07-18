#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H
#include "Component.h"
#include <string>
using namespace std;
class TestComponent: public Component {
public:
    int value = 0;
    string name = "default";
};
#endif