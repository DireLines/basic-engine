#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Vector2.h"
using namespace std;
class Transform: public Component {
public:
    Vector2 position;
    double rotation;
    Vector2 scale;
};

#endif