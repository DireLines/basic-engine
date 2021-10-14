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

Vector2& Vector2::operator+=(const Vector2& rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

Vector2 Vector2::operator*(double s) {
    return Vector2(x * s, y * s);
}
Vector2 Vector2::operator-() {
    return Vector2(-x, -y);
}

void Vector2::print() const {
    cout << "(" << x << ", " << y << ")" << endl;
}

double Vector2::magnitude() const {
    return sqrt(sqrMagnitude());
}

double Vector2::sqrMagnitude() const {
    return x * x + y * y;
}

double Vector2::dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

double Vector2::distance(Vector2 a, Vector2 b) {
    return (b - a).magnitude();
}
double Vector2::calculateRotation(Vector2 origin, Vector2 p) {
    double y = p.y - origin.y;
    double x = p.x - origin.x;
    return atan2(y, x);
}


Vector2 Vector2::normalized() {
    double m = this->magnitude();
    return Vector2(x / m, y / m);
}

Vector2 Vector2::clamp(double amount) {
    if (magnitude() > amount) {
        return normalized() * amount;
    }
    return *this;
}
SDL_Point Vector2::toPixel() {
    return {(int)round(x), (int)round(y)};
}

