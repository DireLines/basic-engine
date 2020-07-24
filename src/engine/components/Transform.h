#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Vector2.h"
#include "Matrix3.h"
using namespace std;
class Transform: public Component {
public:
    Vector2 position = Vector2(0, 0);
    double rotation = 0;
    Vector2 scale = Vector2(1, 1);
    Matrix3 toMatrix();
    static Matrix3 Translate(double x, double y);
    static Matrix3 Translate(Vector2 v);
    static Matrix3 Rotate(double r);
    static Matrix3 Scale(double x, double y);
    static Matrix3 Scale(Vector2 v);
    static Matrix3 toMatrix(Transform& t);
};

#endif