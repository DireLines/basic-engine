#ifndef POLYGON_H
#define POLYGON_H
#include "Vector2.h"
#include <vector>
using namespace std;
class Polygon {
public:
    vector<Vector2> points;

    static Vector2 support(Vector2 direction, vector<Vector2>& vertices);
    Vector2 support(Vector2 direction);

};

#endif
