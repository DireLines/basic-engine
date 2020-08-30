#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2.h"
using namespace std;
class Rigidbody: public Component {
public:
    //TODO: should center_of_mass be a separate thing from Transform.pivot?
    //currently, it is not

    double mass = 1.0;
    double momentOfInertia = 1.0;

    Vector2 velocity = Vector2();
    double angularVelocity = 0.0;

    Vector2 force = Vector2();
    double torque = 0.0;

    //assumes adding force at center of mass
    void addForce(Vector2 f) {
        force += f;
    }
    void addForceAtPoint(Vector2 f, Vector2 p);//adds force at a particular point p, computing the torque/force components
    void addTorque(double t) {
        torque += t;
    }
};
#endif