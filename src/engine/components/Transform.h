#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "MathUtils.h"
using namespace std;
class Transform: public Component {
public:
    Vector2 position = Vector2(0, 0);
    double rotation = 0;
    Vector2 scale = Vector2(1, 1);
    //TODO: I want to set pivot to the midpoint of the sprite most of the time
    //but you should be able to specify otherwise. How?
    Vector2 pivot = Vector2(0, 0);

    Matrix3 toMatrix();
    Matrix3 Apply();
    Matrix3 Reverse();
    Matrix3 Unpivot();
    static Matrix3 Translate(double x, double y);
    static Matrix3 Translate(Vector2 v);
    static Matrix3 Rotate(double r);
    static Matrix3 Scale(double x, double y);
    static Matrix3 Scale(Vector2 v);
    static Matrix3 Apply(Transform& t);
    static Matrix3 Reverse(Transform& t);
};

#endif