#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>
#include "Vector2.h"
using namespace std;
class Matrix3 {
public:
    Matrix3();
    ~Matrix3();
    Matrix3 operator*(Matrix3 const &other) const;
    Vector2 operator*(Vector2 const &other) const;
    Matrix3& operator=(const Matrix3& other);
    void print() const;
private:
    double** identity();
    double** m;
    friend class Transform;
};


#endif