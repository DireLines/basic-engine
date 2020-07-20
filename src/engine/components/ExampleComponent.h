#ifndef EXAMPLECOMPONENT_H
#define EXAMPLECOMPONENT_H

#include <string>
using namespace std;
class ExampleComponent: public Component {
public:
    int value = 0;
    string name = "default";
};
#endif