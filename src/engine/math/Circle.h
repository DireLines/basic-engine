#ifndef CIRCLE_H
#define CIRCLE_H
#include "Vector2.h"
using namespace std;
class Circle {
public:
    Vector2 center;
    double radius;
    Circle();
    Circle(Vector2 c, double r);
    Circle(double x, double y, double r);
};

#endif
