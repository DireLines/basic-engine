#include "CollisionSystem.h"
#include "Game.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}

void CollisionSystem::start() {
    for (int i = 0; i < 361; ++i) {
        Point* p = new Point();
        plotpoints.push_back(p);
        Game::instance->instantiate(p);
    }
}

//TODO: complete broad phase to reduce to O(n log n)
void CollisionSystem::update() {
    sort_endpoints();
    vector<Matrix3> matrices(objects.size());
    for (int i = 0; i < objects.size(); ++i) {
        matrices[i] = objects[i]->transform->Apply();
    }
    bool coll = GJK_collide(objects[0], objects[1]);
    cout << (coll ? "colliding" : "not colliding") << endl;
    MinkowskiDifferenceSupport s(objects[0], objects[1]);
    for (int i = 0; i < 360; ++i) {
        Vector2 dir = Transform::Rotate(MathUtils::deg2rad(i)) * Vector2(1, 0);
        Vector2 v = s(dir);
        plotpoints[i]->getComponent<Transform>()->position = v;
    }
    plotpoints[360]->getComponent<Transform>()->position = Vector2(0, 0);
    for (int i = 0; i < objects.size(); ++i) {
        ColliderTransform* A = objects[i];
        Matrix3& a_mat = matrices[i];
        Collider* a_col = A->collider;
        for (int j = i + 1; j < objects.size(); ++j) {
            ColliderTransform* B = objects[j];
            Matrix3& b_mat = matrices[j];
            Collider* b_col = B->collider;
            // MinkowskiDifferenceSupport s(a_mat, a_col, b_mat, b_col);
            // Vector2 dir(1, 0);
            // s(dir);
        }
    }
}
bool CollisionSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Collider>() && obj->hasComponent<Transform>();
}
void CollisionSystem::addObject(GameObject* obj) {
    Transform* t = obj->getComponent<Transform>();

    for (Collider* c : obj->getComponents<Collider>()) {
        ColliderTransform* ct = new ColliderTransform();
        ct->collider = c;
        ct->transform = t;
        objects.push_back(ct);
    }
}
void CollisionSystem::removeObject(GameObject* obj) {
    vector<ColliderTransform*>::iterator it = objects.begin();
    while (it != objects.end()) {
        if ((*it)->transform->gameObject == obj) {
            delete *it;
            it = objects.erase(it);
        } else {
            it++;
        }
    }
}

//is p in the "tube" perpendicular to the line segment between A and B?
bool inTube(Vector2 p, Vector2 A, Vector2 B) {
    return MathUtils::acute(A, B, p) && MathUtils::acute(B, A, p);
}
//is p in the region bordering the edge AB in the triangle ABC?
//(that is, in the AB "tube" and also not on the other sides of the triangle)
bool inEdgeRegionAB(Vector2 p, Vector2 A, Vector2 B, Vector2 C) {
    if (!MathUtils::sameSideOfLine(p, C, A, B) && inTube(p, A, B)) {
        return true;
    }
    return false;
}

bool CollisionSystem::GJK_collide(ColliderTransform* a, ColliderTransform* b) {
    MinkowskiDifferenceSupport s(a, b);
    Vector2 origin(0, 0);
    Vector2 p1 = s(Random::unitVector());
    Vector2 p2 = s(-p1);
    int iterations = 0;
    if (MathUtils::sameHalfSpace(p1, p2)) {
        cout << "iterations: " << iterations << endl;
        return false;
    }
    while (true) {
        iterations++;
        Vector2 perp = MathUtils::perpendicularTowardOrigin(p1, p2);
        Vector2 p3 = s(perp);
        if (!MathUtils::sameHalfSpace(perp, p3)) {
            cout << "iterations: " << iterations << endl;
            return false;
        }
        if (MathUtils::PointInTriangle(origin, p1, p2, p3)) {
            cout << "iterations: " << iterations << endl;
            return true;
        }
        //determine whether to keep p1's value, replace it with p2's, or recreate both p1 and p2
        bool edge_p3p1 = inEdgeRegionAB(origin, p3, p1, p2);
        bool edge_p3p2 = inEdgeRegionAB(origin, p3, p2, p1);
        //swap points to converge triangle on origin
        Vector2 tmp = p2;
        p2 = p3;
        if (edge_p3p2) {//origin was in region of edge p3p2, so give p1 p2's former value
            p1 = tmp;
        } else if (!edge_p3p1) {//origin was in region of vertex p3
            p1 = p3;
            p2 = s(-p1);
            if (MathUtils::sameHalfSpace(p1, p2)) {
                cout << "iterations: " << iterations << endl;
                return false;
            }
        }
        //else, values are what they should be
    }
}
bool CollisionSystem::colliding(GameObject* a, GameObject* b) {
    return false;
}

void CollisionSystem::resolveCollision(GameObject* a, GameObject* b) {
}

bool pos(IntervalEndpoint* a, IntervalEndpoint* b) {
    return a->pos < b->pos;
}

void CollisionSystem::sort_endpoints() {
    std::sort(endpoints.begin(), endpoints.end(), pos);
}