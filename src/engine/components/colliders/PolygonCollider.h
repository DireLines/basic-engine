#ifndef POLYGONCOLLIDER_H
#define POLYGONCOLLIDER_H
#include "Collider.h"
#include "Polygon.h"
using namespace std;
class PolygonCollider: public Collider {
public:
    Polygon vertices;
    Vector2 support(Vector2 direction) {
        return direction;
    }
};
#endif