#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H
using namespace std;
#include <vector>
#include <memory>
#include "Matrix3.h"

#include <iostream>
class MatrixPool {
public:
    double** create();
    void release(double** ptr);
private:
    vector<double**> pool;
    vector<bool> inUse;
};

#endif