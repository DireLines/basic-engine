#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H
#include "Collider.h"
using namespace std;
class BoxCollider: public Collider {
public:
    double width = 1;
    double height = 1;
    Vector2 support(Vector2 direction) {
        return direction;
    }
};
#endif