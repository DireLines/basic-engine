#ifndef TESTSCRIPT_H
#define TESTSCRIPT_H

#include "Script.h"

using namespace std;
class TestScript: public Script {
public:
    int i;
    void start();
    void update();
};

#endif