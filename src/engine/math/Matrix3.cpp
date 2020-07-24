#include "Matrix3.h"
Matrix3::Matrix3() {
    m = identity();
}
Matrix3::~Matrix3() {
    for (int i = 0; i < 3; i++)
        delete m[i];
    delete m;
}
double** Matrix3::identity() {
    double **id = new double *[3];
    for (int i = 0; i < 3; i++) {
        id[i] = new double[3];
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
    //zero out result
    result.m[0][0] = 0;
    result.m[1][1] = 0;
    result.m[2][2] = 0;
    //perform multiplication
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
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


void Matrix3::print() const {
    printf("[\n");
    printf("\t%4.2f %4.2f %4.2f\n", m[0][0], m[0][1], m[0][2]);
    printf("\t%4.2f %4.2f %4.2f\n", m[1][0], m[1][1], m[1][2]);
    printf("\t%4.2f %4.2f %4.2f\n", m[2][0], m[2][1], m[2][2]);
    printf("]\n");
}