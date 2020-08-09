#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include "Script.h"
class VectorField: public Script {
public:
    Transform* t;
    Rigidbody* rb;

    void start() {
        t = gameObject->getComponent<Transform>();
        rb = gameObject->getComponent<Rigidbody>();
    }

    Vector2 f(Vector2 xy) {
        double x = xy.x / 30;
        double y = xy.y / 30;
        return Vector2(x * x - y * y - 4 , 2 * x * y);
    }

    void update() {
        rb->velocity = f(t->position);
    }
    bool parallelizable() {
        return true;
    }
};

#endif