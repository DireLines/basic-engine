#ifndef COLLIDER_H
#define COLLIDER_H

#include "Polygon.h"
using namespace std;
class Collider: public Component {
public:
    Polygon region;
};
#endif