#ifndef SEEK_H
#define SEEK_H

#include "Script.h"
class Seek: public Script {
public:
    Transform* target;

    double max_velocity = 100;
    double max_force = 1000;

    void start() {
        transform = gameObject->getComponent<Transform>();
        rb = gameObject->getComponent<Rigidbody>();
    }

    void update() {
        Vector2 desired_velocity = (target->position - transform->position).normalized() * max_velocity;
        Vector2 steering_force = (desired_velocity - rb->velocity).clamp(max_force);
        rb->addForce(steering_force);
    }
    bool parallelizable() {
        return true;
    }
private:
    Transform* transform;
    Rigidbody* rb;
};

#endif