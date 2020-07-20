#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2.h"
using namespace std;
class Transform: public Component {
public:
    Vector2 position = Vector2(0, 0);
    double rotation = 0;
    Vector2 scale = Vector2(1, 1);
};

#endif