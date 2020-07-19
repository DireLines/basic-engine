#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include "Component.h"
#include "Vector2.h"
using namespace std;
class Rigidbody: public Component {
public:
    Vector2 center_of_mass = Vector2(0, 0);

    double mass = 1.0;
    Vector2 velocity = Vector2(0, 0);
    Vector2 force = Vector2(0, 0);

    double moment_of_inertia = 1.0;
    Vector2 angularVelocity = Vector2(0, 0);
    Vector2 torque = Vector2(0, 0);
};
#endif