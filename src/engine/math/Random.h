#ifndef RANDOM_H
#define RANDOM_H

class Random {
public:
    static double range(double lowerLimit, double upperLimit) {
        if (lowerLimit > upperLimit) {
            std::swap(lowerLimit, upperLimit);
        }
        return ((double)rand() / RAND_MAX) * (upperLimit - lowerLimit) + lowerLimit;
    }
    static double normal(double mean, double stddev) {
        return standard_gaussian() * stddev + mean;
    }
    static double standard_gaussian() {
        //TODO: sample from standard gaussian
        return 1;
    }
    static Vector2 unitVector() {
        return Vector2(standard_gaussian(), standard_gaussian()).normalized();
    }
    static Vector2 insideUnitCircle() {
        return unitVector() * range(0, 1);
    }
};

#endif