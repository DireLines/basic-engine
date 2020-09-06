#ifndef SCATTER_H
#define SCATTER_H

#include "Script.h"
class Scatter: public Script {
public:
    double range = 500;
    double rotationRange = 180;
    double maxSpeed = 100;
    void start() {
        Transform* t = gameObject->getComponent<Transform>();
        t->rotation = Random::range(-rotationRange, rotationRange);
        t->position = Vector2(Random::range(-range, range), Random::range(-range, range));
        gameObject->getComponent<Rigidbody>()->velocity =
            Vector2(Random::range(-maxSpeed, maxSpeed), Random::range(-maxSpeed, maxSpeed));
        gameObject->getComponent<Rigidbody>()->angularVelocity = Random::range(-maxSpeed, maxSpeed);
    }
    bool parallelizable() {
        return true;
    }
};

#endif