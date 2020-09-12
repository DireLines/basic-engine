#include "CollisionSystem.h"
MinkowskiDifferenceSupport::MinkowskiDifferenceSupport(ColliderTransform* A, ColliderTransform* B) {
    A_transform = A->transform->Apply();
    A_collider = A->collider;
    A_rotation = Transform::Rotate(-A_transform.rotation());
    B_transform = B->transform->Apply();
    B_collider = B->collider;
    B_rotation = Transform::Rotate(-B_transform.rotation());
}
MinkowskiDifferenceSupport::MinkowskiDifferenceSupport(ColliderMatrices& A, ColliderMatrices& B) {
    A_transform = A.applied_transform;
    A_collider = A.collider;
    A_rotation = A.undo_rotation;
    B_transform = B.applied_transform;
    B_collider = B.collider;
    B_rotation = B.undo_rotation;
}
MinkowskiDifferenceSupport::~MinkowskiDifferenceSupport() {
}

Vector2 MinkowskiDifferenceSupport::operator()(Vector2 direction) {
    return transformedSupport(direction, A_transform, A_collider, A_rotation) - transformedSupport(-direction, B_transform, B_collider, B_rotation);
}
Vector2 MinkowskiDifferenceSupport::transformedSupport(Vector2 direction, Matrix3& t, Collider* collider) {
    Matrix3 m = Transform::Rotate(-t.rotation());
    return transformedSupport(direction, t, collider, m);
}
Vector2 MinkowskiDifferenceSupport::transformedSupport(Vector2 direction, Matrix3& t, Collider* collider, Matrix3& rotation) {
    return t * collider->support(rotation * direction);
};