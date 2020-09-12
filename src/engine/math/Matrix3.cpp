#include "Matrix3.h"
//row-major indexing
#define IDX(r,c) ((r)*DIM + (c))

Matrix3::Matrix3() {
    init_to_identity();
}

double* Matrix3::operator[](int r) {
    return m + DIM * r;
}
void Matrix3::init_to_identity() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (i == j)
                m[IDX(i, j)] = 1.0;
            else
                m[IDX(i, j)] = 0.0;
        }
    }
}

//matrix multiply
Matrix3 Matrix3::operator*(Matrix3 const &other) const {
    Matrix3 result;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            result.m[IDX(i, j)] = 0;
            for (int k = 0; k < DIM; k++) {
                result.m[IDX(i, j)] += this->m[IDX(i, k)] * other.m[IDX(k, j)];
            }
        }
    }
    return result;
}

//matrix vector product
Vector2 Matrix3::operator*(Vector2 const &other) const {
    Vector2 result;
    result.x = (other.x * m[IDX(0, 0)]) +
               (other.y * m[IDX(0, 1)]) +
               (1 * m[IDX(0, 2)]);
    result.y = (other.x * m[IDX(1, 0)]) +
               (other.y * m[IDX(1, 1)]) +
               (1 * m[IDX(1, 2)]);
    return result;
}

double Matrix3::rotation() {
    return Vector2::calculateRotation(Vector2(0, 0), (*this) * Vector2(1, 0) - (*this) * Vector2(0, 0));
}

void Matrix3::print() const {
    printf("[\n");
    for (int i = 0; i < DIM; ++i) {
        printf("\t");
        for (int j = 0; j < DIM; ++j) {
            printf("%4.2f ", m[IDX(i, j)]);
        }
        printf("\n");
    }
    printf("]\n");
}