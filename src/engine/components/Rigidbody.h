#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2.h"
using namespace std;
class Rigidbody: public Component {
public:
    double mass = 1.0;
    double momentOfInertia = 1.0;

    Vector2 velocity = Vector2();
    Vector2 angularVelocity = Vector2();

    Vector2 force = Vector2();
    Vector2 torque = Vector2();
};
#endif