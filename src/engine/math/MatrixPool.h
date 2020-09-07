#ifndef MATRIXPOOL_H
#define MATRIXPOOL_H
using namespace std;
#include <vector>
#include "Matrix3.h"

class MatrixPool {
public:
    double** create();
    void release(double** ptr);
private:
    vector<double**> pool;
    vector<bool> inUse;
};

#endif