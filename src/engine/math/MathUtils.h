#ifndef MATHUTILS_H
#define MATHUTILS_H

#define PI 3.14159265358979
class MathUtils {
public:
    static double rad2deg(double rads) {
        return rads * (180 / PI);
    }
    static double deg2rad(double degs) {
        return degs * (PI / 180);
    }
};
#endif