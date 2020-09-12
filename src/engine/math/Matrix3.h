#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>
#include "Vector2.h"
using namespace std;
class Matrix3 {
public:
    Matrix3();
    Matrix3 operator*(Matrix3 const &other) const;//matrix multiply
    Vector2 operator*(Vector2 const &other) const;//matrix vector product

    /* how much rotation does this transformation cause?
    NOTE: if the transformation shears points, the amount of rotation is not constant
    this returns the amount of rotation on the arbitrary point (1, 0)*/
    double rotation();

    void print() const;
private:
    double* operator[](int r);//index into the matrix
    void init_to_identity();
    static const int DIM = 3;
    double m[DIM * DIM];
    friend class Transform;
};


#endif