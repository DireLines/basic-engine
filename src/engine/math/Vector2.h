#ifndef VECTOR2_H
#define VECTOR2_H
#include <SDL2/SDL.h>
#include <iostream>
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
    Vector2& operator+=(const Vector2& rhs);
    Vector2& operator-=(const Vector2& rhs);
    SDL_Point toPixel();
    void print() const;
    double magnitude() const;
    double sqrMagnitude() const;
    static double distance(Vector2 a, Vector2 b);
    static double calculateRotation(Vector2 origin, Vector2 p);
    Vector2 operator *(double s);
    Vector2 normalized();
};

#endif
