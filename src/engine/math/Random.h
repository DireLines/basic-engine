#ifndef RANDOM_H
#define RANDOM_H

#include "MathUtils.h"

class Random {
public:
    static double range(double lowerLimit, double upperLimit) {
        if (lowerLimit > upperLimit) {
            std::swap(lowerLimit, upperLimit);
        }
        return ((double)rand() / RAND_MAX) * (upperLimit - lowerLimit) + lowerLimit;
    }
    //TODO: sample from standard gaussian
    //(using CLT seems to be the usual way)
    // static double standard_gaussian() {
    //     return 1;
    // }
    // static double normal(double mean, double stddev) {
    //     return standard_gaussian() * stddev + mean;
    // }
    // static Vector2 unitVector() {
    //     return Vector2(standard_gaussian(), standard_gaussian()).normalized();
    // }

    static Vector2 unitVector() {
        return Transform::Rotate(range(0, 360)) * Vector2(1, 0);
    }
    static Vector2 insideUnitCircle() {
        return unitVector() * sqrt(range(0, 1));
    }
};

#endif