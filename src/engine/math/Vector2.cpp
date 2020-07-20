#include <Vector2.h>
Vector2::Vector2() {
    x = 0;
    y = 0;
}

Vector2::Vector2(SDL_Point a) : x(a.x), y(a.y) {};

Vector2::Vector2(double _x, double _y) {
    x = _x;
    y = _y;
}

Vector2 Vector2::operator+(Vector2 const &other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(Vector2 const &other) const {
    return Vector2(x - other.x, y - other.y);
}

void Vector2::print() const { cout << "(" << x << ", " << y << ")" << endl; }

double Vector2::magnitude() const { return sqrt(sqrMagnitude()); }

double Vector2::sqrMagnitude() const { return x * x + y * y; }
