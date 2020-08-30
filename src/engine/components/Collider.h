#ifndef COLLIDER_H
#define COLLIDER_H
#include "Vector2.h"
using namespace std;
class Collider: public Component {
public:
    //if isTrigger is set to true, then the collider will not resolve collisions, merely call OnCollision functions
    bool isTrigger = false;

    //the Support Function used in Gilbert-Johnson-Keerthi collision detection
    //returns the point on this collider shape which has
    //highest dot product with input direction
    virtual Vector2 support(Vector2 direction) {
        return direction;//please override this, this answer is bogus
    }
};
#endif