#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "Vector2.h"

#define PI 3.14159265358979
class MathUtils {
public:
    static double rad2deg(double rads) {
        return rads * (180 / PI);
    }
    static double deg2rad(double degs) {
        return degs * (PI / 180);
    }
    //stolen from https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
    static bool PointInTriangle(Vector2 pt, Vector2 v1, Vector2 v2, Vector2 v3) {
        double d1, d2, d3;
        bool has_neg, has_pos;
        d1 = determinant(pt, v1, v2);
        d2 = determinant(pt, v2, v3);
        d3 = determinant(pt, v3, v1);
        has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
        return !(has_neg && has_pos);
    }
private:
    //stolen from https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
    static double determinant(Vector2 p1, Vector2 p2, Vector2 p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }
};
#endif