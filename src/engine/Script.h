#ifndef SCRIPT_H
#define SCRIPT_H

#include "Component.h"
using namespace std;
class Script: public Component {
public:
    Script() {}
    ~Script() {}
    virtual void start() {}
    virtual void update() {}
};

#endif