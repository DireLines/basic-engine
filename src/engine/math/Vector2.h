#ifndef VECTOR2_H
#define VECTOR2_H
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
using namespace std;
class Vector2 {
public:
    double x;
    double y;
    Vector2();
    Vector2(SDL_Point a);
    Vector2(double _x, double _y);
    Vector2 operator+(Vector2 const &other) const;
    Vector2 operator-(Vector2 const &other) const;
    void print() const;
    double magnitude() const;
    double sqrMagnitude() const;
};

#endif
