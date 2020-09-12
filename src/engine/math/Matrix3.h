#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>
#include "Vector2.h"
using namespace std;
class Matrix3 {
public:
    Matrix3();
    Matrix3 operator*(Matrix3 const &other) const;
    Vector2 operator*(Vector2 const &other) const;
    // Matrix3& operator=(const Matrix3& other);
    double rotation();
    void print() const;
private:
    double* operator[](int r);
    void init_to_identity();
    static const int DIM = 3;
    double m[DIM * DIM];
    friend class Transform;
};


#endif