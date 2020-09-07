#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H
#include "Collider.h"
#include "Polygon.h"
using namespace std;
class BoxCollider: public Collider {
public:
    double width = 1;
    double height = 1;
    BoxCollider() {
        double w = width / 2;
        double h = height / 2;
        corners.push_back(Vector2(w, h));
        corners.push_back(Vector2(w, -h));
        corners.push_back(Vector2(-w, -h));
        corners.push_back(Vector2(-w, h));
    }
    Vector2 support(Vector2 direction) {
        double w = width / 2;
        double h = height / 2;
        corners[0] = Vector2(w, h);
        corners[1] = Vector2(w, -h);
        corners[2] = Vector2(-w, -h);
        corners[3] = Vector2(-w, h);
        return Polygon::support(direction, corners);
    }
private:
    vector<Vector2> corners;
};
#endif