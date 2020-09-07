#include "Transform.h"

Matrix3 Transform::toMatrix() {
    return Apply(*this);
}

Matrix3 Transform::Apply() {
    return Apply(*this);
}
Matrix3 Transform::Reverse() {
    return Reverse(*this);
}
Matrix3 Transform::Unpivot() {
    return Translate(-pivot.x, -pivot.y);
}
Matrix3 Transform::Translate(double x, double y) {
    Matrix3 result;
    result.m[0][2] = x;
    result.m[1][2] = y;
    return result;
}
Matrix3 Transform::Translate(Vector2 v) {
    return Translate(v.x, v.y);
}
Matrix3 Transform::Rotate(double r) {
    Matrix3 result;
    result.m[0][0] = cos(r);
    result.m[0][1] = -sin(r);
    result.m[1][0] = sin(r);
    result.m[1][1] = cos(r);
    return result;
}
Matrix3 Transform::Scale(double x, double y) {
    Matrix3 result;
    result.m[0][0] = x;
    result.m[1][1] = y;
    return result;
}
Matrix3 Transform::Scale(Vector2 v) {
    return Scale(v.x, v.y);
}
Matrix3 Transform::Apply(Transform& t) {
    return Translate(t.position) *
           Rotate(MathUtils::deg2rad(t.rotation)) *
           Scale(t.scale);
}

Matrix3 Transform::Reverse(Transform& t) {
    return Scale(1.0 / t.scale.x, 1.0 / t.scale.y) *
           Rotate(MathUtils::deg2rad(-t.rotation)) *
           Translate(-t.position.x, -t.position.y);
}