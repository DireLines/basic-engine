#ifndef TESTSCRIPT_H
#define TESTSCRIPT_H

#include "Script.h"

class TestScript: public Script {
public:
    void start();
    void update();
private:
    int i;
};

#endif