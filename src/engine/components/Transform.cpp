#include "Transform.h"

Matrix3 Transform::toMatrix() {
    return toMatrix(*this);
}
Matrix3 Transform::Translate(double x, double y) {
    Matrix3 result;
    result.m[0][2] = x;
    result.m[1][2] = y;
    return result;
}
Matrix3 Transform::Translate(Vector2 v) {
    Matrix3 result;
    result.m[0][2] = v.x;
    result.m[1][2] = v.y;
    return result;
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
    Matrix3 result;
    result.m[0][0] = v.x;
    result.m[1][1] = v.y;
    return result;
}
Matrix3 Transform::toMatrix(Transform& t) {
    return Translate(t.position) * Rotate(t.rotation) * Scale(t.scale);
}