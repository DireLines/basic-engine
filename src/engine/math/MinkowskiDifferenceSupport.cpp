#include "CollisionSystem.h"
MinkowskiDifferenceSupport::MinkowskiDifferenceSupport(ColliderTransform* A, ColliderTransform* B) {
    A_transform = A->transform->Apply();
    A_collider = A->collider;
    A_rotation = Transform::Rotate(-A_transform.rotation());
    B_transform = B->transform->Apply();
    B_collider = B->collider;
    B_rotation = Transform::Rotate(-B_transform.rotation());
}
MinkowskiDifferenceSupport::MinkowskiDifferenceSupport(Matrix3& a_mat, Collider* a_col, Matrix3& b_mat, Collider* b_col) {
    A_transform = a_mat;
    A_collider = a_col;
    A_rotation = Transform::Rotate(-A_transform.rotation());
    B_transform = b_mat;
    B_collider = b_col;
    B_rotation = Transform::Rotate(-B_transform.rotation());
}
MinkowskiDifferenceSupport::~MinkowskiDifferenceSupport() {
}

Vector2 MinkowskiDifferenceSupport::operator()(Vector2 direction) {
    return transformedSupport(direction, A_transform, A_collider, A_rotation) - transformedSupport(-direction, B_transform, B_collider, B_rotation);
}
Vector2 MinkowskiDifferenceSupport::transformedSupport(Vector2 direction, Matrix3& t, Collider* collider) {
    double rotation = t.rotation();
    return t * collider->support(Transform::Rotate(-rotation) * direction);
}
Vector2 MinkowskiDifferenceSupport::transformedSupport(Vector2 direction, Matrix3& t, Collider* collider, Matrix3& rotation) {
    return t * collider->support(rotation * direction);
};