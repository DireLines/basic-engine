#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "Vector2.h"
#include "Circle.h"

const double PI = 3.14159265358979;
class MathUtils {
public:
    static double rad2deg(double rads) {
        return rads * (180 / PI);
    }
    static double deg2rad(double degs) {
        return degs * (PI / 180);
    }
    //stolen from https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
    static bool PointInTriangle(Vector2 pt, Vector2 v1, Vector2 v2, Vector2 v3) {
        double d1, d2, d3;
        bool has_neg, has_pos;
        d1 = determinant(pt, v1, v2);
        d2 = determinant(pt, v2, v3);
        d3 = determinant(pt, v3, v1);
        has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
        return !(has_neg && has_pos);
    }

    //are a and b on the same side of the origin?
    static bool sameHalfSpace(Vector2 a, Vector2 b) {
        return Vector2::dot(a, b) > 0;
    }

    //is abc an acute angle?
    static bool acute(Vector2 a, Vector2 b, Vector2 c) {
        return Vector2::dot(a - b, c - b) > 0;
    }

    //are p1 and p2 on the same side of the line AB?
    static bool sameSideOfLine(Vector2 p1, Vector2 p2, Vector2 A, Vector2 B) {
        Vector2 perp = perpendicular(A, B);
        return (sameHalfSpace(perp, A - p1) == sameHalfSpace(perp, A - p2));
    }

    //a vector perpendicular to the line ab
    static Vector2 perpendicular(Vector2 a, Vector2 b) {
        Vector2 diff = b - a;
        return Vector2(-diff.y, diff.x);
    }
    //a vector perpendicular to the line ab in the direction of the origin
    //very useful for collision detection algorithm
    static Vector2 perpendicularTowardOrigin(Vector2 a, Vector2 b) {
        Vector2 perp = perpendicular(a, b);
        if (sameHalfSpace(perp, a)) {
            perp = -perp;
        }
        return perp;
    }
    //does the interval a1 to a2 overlap with the interval b1 to b2?
    static bool intervalOverlap(double a1, double a2, double b1, double b2) {
        if (a1 > a2) {
            std::swap(a1, a2);
        }
        if (b1 > b2) {
            std::swap(b1, b2);
        }
        return a1 <= b2 && b1 <= a2;
    }

    //helper function for threads to compute their portion of an array given thread id
    static int getThreadStartIndex(int start, int stop, int threadnum, int num_threads) {
        return (int)((stop - start) * ((double)threadnum / num_threads) + start);
    }

    static Vector2 transpose(Vector2 v) {
        return Vector2(v.y, v.x);
    }

    static Vector2 closestPointToOriginOnLine(Vector2 p1, Vector2 p2) {
        Vector2 diff = p2 - p1;
        return  diff * (Vector2::dot(diff, -p1)/diff.sqrMagnitude()) + p1;
    }

    // static tuple<double, double> quadratic_formula(double a, double b, double c) {
    //     double underRoot = b * b - 4 * a * c;
    //     if (underRoot < 0) {//no real solutions
    //         return NULL;
    //     }
    //     double root = sqrt(underRoot);
    //     double x1 = (-b + root) / (2 * a);
    //     double x2 = (-b - root) / (2 * a);
    //     return {x1, x2};
    // }

    // static set<Vector2> circle_intersections(Circle a, Circle b) {
    //     set<Vector2> result;
    //     double xa = a.center.x; double ya = a.center.y;  double ra = a.radius;
    //     double xb = b.center.x; double yb = b.center.y;  double rb = b.radius;

    //     //edge cases
    //     if (xa == xb) {
    //         if (ya == yb) {
    //             return NULL;//circles centered on the same point have either zero or infinite points in common
    //         }
    //         //transpose so that denom is not zero
    //         set<Vector2> transposed_intersections = circle_intersections(transpose(a), transpose(b));
    //         if (transposed_intersections == NULL) {
    //             return NULL;
    //         }
    //         foreach (Vector2 v in transposed_intersections) {
    //             result.Add(transpose(v));
    //         }
    //         return result;
    //     }

    //     //intermediate terms to simplify formulas
    //     double denom = 2 * (xa - xb);
    //     double D = (rb * rb - ra * ra + ya * ya - yb * yb + xa * xa - xb * xb) / denom;
    //     double E = 2 * (yb - ya) / denom;
    //     double F = D - xa;

    //     //coefficients to pass to quadratic formula
    //     double A = E * E + 1;
    //     double B = 2 * E * F - 2 * ya;
    //     double C = F * F + ya * ya - ra * ra;

    //     tuple<double, double> ys = quadratic_formula(A, B, C);
    //     //no solutions
    //     if (ys == NULL) {
    //         return NULL;
    //     }

    //     double y1 = ys.Item1; double y2 = ys.Item2;
    //     double x1 = D + E * y1; double x2 = D + E * y2;
    //     result.add(Vector2(x1, y1));
    //     result.add(Vector2(x2, y2));
    //     return result;
    // }
    // static set<Vector2> tangent_points(Circle circle, Vector2 p) {
    //     Vector2 halfway = (p + circle.center) * 0.5f;
    //     double radius = (circle.center - p).magnitude() * 0.5f;
    //     return circle_intersections(circle, Circle(halfway, radius));
    // }
private:
    //stolen from https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
    static double determinant(Vector2 p1, Vector2 p2, Vector2 p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }

    //helper for circle intersection edge case
    static Circle transpose(Circle c) {
        return Circle(transpose(c.center), c.radius);
    }
};
#endif