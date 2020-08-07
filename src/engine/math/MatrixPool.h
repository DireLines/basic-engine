#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H
using namespace std;
#include <vector>
#include <memory>
#include "Matrix3.h"

#include <iostream>
class MatrixPool {
public:
    double** create() {
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
    void release(double** ptr) {
        for (int i = 0; i < pool.size(); ++i) {
            if (pool[i] == ptr) {
                inUse[i] = false;
            }
        }
    }
private:
    vector<double**> pool;
    vector<bool> inUse;
};

#endif