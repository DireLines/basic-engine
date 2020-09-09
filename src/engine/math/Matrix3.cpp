#include "Matrix3.h"
#include "MatrixPool.h"

static MatrixPool matrixPool;
Matrix3::Matrix3() {
    m = identity();
}
Matrix3::~Matrix3() {
    matrixPool.release(m);
}
double** Matrix3::identity() {
    double **id = matrixPool.create();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j)
                id[i][j] = 1.0;
            else
                id[i][j] = 0.0;
        }
    }
    return id;
}

//matrix multiply
Matrix3 Matrix3::operator*(Matrix3 const &other) const {
    Matrix3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result.m[i][j] += this->m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

//matrix vector product
Vector2 Matrix3::operator*(Vector2 const &other) const {
    Vector2 result;
    result.x = (other.x * m[0][0]) +
               (other.y * m[0][1]) +
               (1 * m[0][2]);
    result.y = (other.x * m[1][0]) +
               (other.y * m[1][1]) +
               (1 * m[1][2]);
    return result;
}

Matrix3& Matrix3::operator=(const Matrix3& other) {
    //self-assignment
    if (&other == this)
        return *this;
    //copy values
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->m[i][j] = other.m[i][j];
        }
    }
    return *this;
}

double Matrix3::rotation() {
    return Vector2::calculateRotation(Vector2(0, 0), (*this) * Vector2(1, 0) - (*this) * Vector2(0, 0));
}

void Matrix3::print() const {
    printf("[\n");
    printf("\t%4.2f %4.2f %4.2f\n", m[0][0], m[0][1], m[0][2]);
    printf("\t%4.2f %4.2f %4.2f\n", m[1][0], m[1][1], m[1][2]);
    printf("\t%4.2f %4.2f %4.2f\n", m[2][0], m[2][1], m[2][2]);
    printf("]\n");
}