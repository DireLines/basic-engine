#ifndef SCATTER_H
#define SCATTER_H

#include "Script.h"
class Scatter: public Script {
public:
    double range = 1000;
    double rotationRange = 180;
    double maxSpeed = 100;
    void start() {
        Transform* t = gameObject->getComponent<Transform>();
        t->rotation = Random::range(-rotationRange, rotationRange);
        t->position = Random::insideUnitCircle() * range;
        gameObject->getComponent<Rigidbody>()->velocity = Random::insideUnitCircle() * maxSpeed;
        gameObject->getComponent<Rigidbody>()->angularVelocity = Random::range(-maxSpeed, maxSpeed);
    }
    bool parallelizable() {
        return true;
    }
};

#endif