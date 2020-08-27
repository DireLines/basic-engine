#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H
#include "Collider.h"
using namespace std;
class CircleCollider: public Collider {
public:
    double radius = 1;
    Vector2 support(Vector2 direction) {
        return direction.normalized() * radius;
    }
};
#endif