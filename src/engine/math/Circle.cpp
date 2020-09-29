#include "Circle.h"

Circle::Circle() {
    center = Vector2();
}
Circle::Circle(Vector2 c, double r) {
    center = c;
    radius = r;
}
Circle::Circle(double x, double y, double r) {
    center = Vector2(x, y);
    radius = r;
}
