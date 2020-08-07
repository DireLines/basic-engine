#include "MatrixPool.h"

double** MatrixPool::create() {
    for (int i = 0; i < pool.size(); ++i) {
        if (!inUse[i]) {
            inUse[i] = true;
            return pool[i];
        }
    }
    double **new_mat = new double *[3];
    for (int i = 0; i < 3; i++) {
        new_mat[i] = new double[3];
    }
    pool.push_back(new_mat);
    inUse.push_back(true);
    return new_mat;
}
void MatrixPool::release(double** ptr) {
    for (int i = 0; i < pool.size(); ++i) {
        if (pool[i] == ptr) {
            inUse[i] = false;
        }
    }
}