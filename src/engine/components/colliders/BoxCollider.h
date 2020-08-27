#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H
#include "Collider.h"
#include "Polygon.h"
using namespace std;
class BoxCollider: public Collider {
public:
    double width = 1;
    double height = 1;
    Vector2 support(Vector2 direction) {
        double w = width / 2;
        double h = height / 2;
        vector<Vector2> corners {
            Vector2(w, h),
            Vector2(w, -h),
            Vector2(-w, -h),
            Vector2(-w, h),
        };
        return Polygon::support(direction, corners);
    }
};
#endif