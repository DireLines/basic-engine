#include "CollisionSystem.h"
MinkowskiDifferenceSupport::MinkowskiDifferenceSupport(ColliderTransform* A, ColliderTransform* B) {
    A_transform = A->transform->Apply();
    A_collider = A->collider;
    B_transform = B->transform->Apply();
    B_collider = B->collider;
}
MinkowskiDifferenceSupport::MinkowskiDifferenceSupport(Matrix3& a_mat, Collider* a_col, Matrix3& b_mat, Collider* b_col) {
    A_transform = a_mat;
    A_collider = a_col;
    B_transform = b_mat;
    B_collider = b_col;
}
MinkowskiDifferenceSupport::~MinkowskiDifferenceSupport() {
}

Vector2 MinkowskiDifferenceSupport::operator()(Vector2 direction) {
    return transformedSupport(direction, A_transform, A_collider) - transformedSupport(direction * -1, B_transform, B_collider);
}
//actually do the minkowski sum, because it's easier to check correctness
Vector2 MinkowskiDifferenceSupport::sum(Vector2 direction) {
    return transformedSupport(direction, A_transform, A_collider) + transformedSupport(direction, B_transform, B_collider);
}
Vector2 MinkowskiDifferenceSupport::transformedSupport(Vector2 direction, Matrix3& t, Collider* collider) {
    double rotation = Vector2::calculateRotation(Vector2(0, 0), t * Vector2(1, 0) - t * Vector2(0, 0));
    Vector2 orig_support = collider->support(Transform::Rotate(-rotation) * direction);
    return t * orig_support;
}