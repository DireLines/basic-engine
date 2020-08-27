#include "Polygon.h"
Vector2 Polygon::support(Vector2 direction, vector<Vector2>& vertices) {
    double highest = -DBL_MAX;
    Vector2 result = Vector2(0, 0);

    for (int i = 0; i < vertices.size(); ++i) {
        Vector2 vertex = vertices[i];
        double dot_product = vertex.x * direction.x + vertex.y * direction.y;
        if (dot_product > highest) {
            highest = dot_product;
            result = vertex;
        }
    }
    return result;
}
Vector2 Polygon::support(Vector2 direction) {
    return support(direction, points);
}
