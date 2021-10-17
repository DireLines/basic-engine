#include "CollisionSystem.h"
#include "Sprite.h" //debug
#include "Game.h"

const auto processor_count = std::thread::hardware_concurrency();

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}

void CollisionSystem::start() {
    //TODO: spawn some threads and put them in a pool to avoid creating new threads evey frame
}

void CollisionSystem::update() {
    precalculate_matrices();
    update_endpoint_positions();
    sort_intervals();
    vector<Collision> collisions = detectCollisions();
    resolveCollisions(collisions);

    //TODO: call collision events
}
#pragma region addremove
bool CollisionSystem::needObject(GameObject* obj) {
    return obj->hasComponent<Collider>() && obj->hasComponent<Transform>();
}
void CollisionSystem::addObject(GameObject* obj) {
    Transform* t = obj->getComponent<Transform>();

    for (Collider* c : obj->getComponents<Collider>()) {
        ColliderTransform ct = ColliderTransform{
            .collider = c,
            .transform = t
        };
        intervals.push_back(Interval{.begin = 0,.end = 0,.object = ct, .precalculated = precalculate(ct)});
    }
}
void CollisionSystem::removeObject(GameObject* obj) {
    vector<Interval>::iterator it = intervals.begin();
    while (it != intervals.end()) {
        if ((*it).object.transform->gameObject == obj) {
            it = intervals.erase(it);
        } else {
            it++;
        }
    }
}
#pragma endregion
#pragma region setup

ColliderMatrices CollisionSystem::precalculate(ColliderTransform ct) {
    Matrix3 t = ct.transform->Apply();
    return ColliderMatrices {
        .collider = ct.collider,
        .applied_transform = t,
        .undo_rotation = Transform::Rotate(-t.rotation())
    };
}
void CollisionSystem::precalculate_matrices() {
    for (Interval &interval : intervals) {
        interval.precalculated = precalculate(interval.object);
    }
}

void CollisionSystem::update_endpoint_positions() {
    for (Interval &interval : intervals) {
        Matrix3 t = interval.precalculated.applied_transform;
        Matrix3 r = interval.precalculated.undo_rotation;
        interval.begin = MinkowskiDifferenceSupport::transformedSupport(Vector2(-1, 0),
                         t, interval.object.collider, r).x;
        interval.end = MinkowskiDifferenceSupport::transformedSupport(Vector2(1, 0),
                       t, interval.object.collider, r).x;
    }
}

bool beginning_pos(Interval a, Interval b) {
    return a.begin < b.begin;
}

void CollisionSystem::sort_intervals() {
    std::sort(intervals.begin(), intervals.end(), beginning_pos);
}
#pragma endregion
#pragma region detection
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

bool CollisionSystem::GJK_collide(ColliderMatrices a, ColliderMatrices b) {
    MinkowskiDifferenceSupport s(a, b);
    Vector2 origin(0, 0);
    Vector2 p1 = s(Vector2(0, 1));
    Vector2 p2 = s(-p1);
    if (MathUtils::sameHalfSpace(p1, p2)) {
        return false;
    }
    while (true) {
        Vector2 perp = MathUtils::perpendicularTowardOrigin(p1, p2);
        Vector2 p3 = s(perp);
        if (!MathUtils::sameHalfSpace(perp, p3)) {
            return false;
        }
        if (MathUtils::PointInTriangle(origin, p1, p2, p3)) {
            return true;
        }
        //swap points to converge triangle on origin
        //by the end, p1 and p2 should be the vertices of the closest edge to the origin
        bool edge_p3p1 = inEdgeRegionAB(origin, p3, p1, p2);
        bool edge_p3p2 = inEdgeRegionAB(origin, p3, p2, p1);
        Vector2 tmp = p2;
        p2 = p3; //definitely include p3, the most recent point
        if (edge_p3p2) {//origin was in region of edge p3p2, so give p1 p2's former value
            p1 = tmp;
        } else if (!edge_p3p1) {//origin was in region of vertex p3
            p1 = p3;
            p2 = s(-p1);
            if (MathUtils::sameHalfSpace(p1, p2)) {
                return false;
            }
        }
        //else, origin was in region of edge p3p1, and values already are what they should be
    }
}

// struct ColliderMatrices {
//     Matrix3 applied_transform;
//     Matrix3 undo_rotation;
//     Collider* collider;
// };

bool CollisionSystem::colliding(ColliderTransform& a, ColliderTransform& b) {
    return GJK_collide(precalculate(a), precalculate(b));
}

void CollisionSystem::detect_collisions_thread(int thread_id, vector<Collision>& output) {
    int start = MathUtils::getThreadStartIndex(0, intervals.size(), thread_id, processor_count);
    int stop = MathUtils::getThreadStartIndex(0, intervals.size(), thread_id + 1, processor_count);
    for (int i = start; i < stop && i < intervals.size(); ++i) {
        ColliderMatrices& o1 = intervals[i].precalculated;
        for (int j = i + 1; j < intervals.size() && intervals[j].begin <= intervals[i].end; j++) {
            ColliderMatrices& o2 = intervals[j].precalculated;
            if (!(o1.collider->enabled && o2.collider->enabled)) {
                continue;
            }
            //you should not be able to collide with yourself
            if (o1.collider->gameObject == o2.collider->gameObject) {
                continue;
            }
            if (GJK_collide(o1, o2)) {
                output.push_back(Collision {
                    ColliderTransform{o1.collider,intervals[i].object.transform},
                    ColliderTransform{o2.collider,intervals[j].object.transform}
                });
            }
        }
    }
}

vector<Collision> CollisionSystem::detectCollisions() {
    vector<vector<Collision>*> threadOutputs;
    vector<thread> threads;
    for (int i = 0; i < processor_count; ++i) {
        threadOutputs.push_back(new vector<Collision>());
        threads.push_back(thread(&CollisionSystem::detect_collisions_thread, this, i, std::ref(*threadOutputs[i])));
    }
    for (int i = 0; i < processor_count; ++i) {
        threads[i].join();
    }
    vector<Collision> collisions;
    for (vector<Collision> *colls : threadOutputs) {
        for (Collision c : *colls) {
            collisions.push_back(c);
        }
    }
    return collisions;
}
#pragma endregion
#pragma region resolution
#define SET_TRANSFORMS(parameter) {\
    transform_A.position = posInitial_A + posDelta_A * (parameter);\
    transform_B.position = posInitial_B + posDelta_B * (parameter);\
    transform_A.rotation = rotInitial_A + rotDelta_A * (parameter);\
    transform_B.rotation = rotInitial_B + rotDelta_B * (parameter);\
}
#define BINARY_SEARCH(output) {\
    /* invariant: colliding at lowerBound, not colliding at upperBound */\
    double lowerBound = 0;\
    double upperBound = 1;\
    const int binarySearchIters = 10;\
    for(int iter = 0; iter < binarySearchIters; iter++) {\
        double midpoint = (lowerBound + upperBound) * 0.5;\
        SET_TRANSFORMS(midpoint);\
        if(colliding(a,b)) {\
            lowerBound = midpoint;\
        }\
        else {\
            upperBound = midpoint;\
        }\
    }\
    (output) = upperBound;\
}
void CollisionSystem::resolveCollision(ColliderTransform& a, ColliderTransform& b) {
    Rigidbody* rb_A = a.transform->gameObject->getComponent<Rigidbody>();
    Rigidbody* rb_B = b.transform->gameObject->getComponent<Rigidbody>();
    if(!(rb_A || rb_B)) {
        //both objects are kinematic - neither can move
        return;
    }
    Transform& transform_A = *a.transform;
    Transform& transform_B = *b.transform;
    double dt = GameTimer::DeltaTime();
    bool move_A = rb_A != NULL;
    bool move_B = rb_B != NULL;
    Vector2 posInitial_A = transform_A.position;
    Vector2 posInitial_B = transform_B.position;
    Vector2 posDelta_A = Vector2();
    Vector2 posDelta_B = Vector2();
    double rotInitial_A = transform_A.rotation;
    double rotInitial_B = transform_B.rotation;
    double rotDelta_A = 0;
    double rotDelta_B = 0;
    Vector2 velocity_A = Vector2();
    Vector2 velocity_B = Vector2();
    double angularVelocity_A = 0;
    double angularVelocity_B = 0;
    if(move_A) {
        velocity_A  = -rb_A->velocity*dt;
        rotDelta_A  = -rb_A->angularVelocity*dt;
    }
    if(move_B) {
        velocity_B = -rb_B->velocity*dt;
        rotDelta_B = -rb_B->angularVelocity*dt;
    }
    // determine which axes to do binary search on
    bool searchX, searchY = false;
    rotDelta_A = 0;
    rotDelta_B = 0;
    posDelta_A = Vector2(velocity_A.x, 0);
    posDelta_B = Vector2(velocity_B.x, 0);
    SET_TRANSFORMS(1);
    searchX = !colliding(a,b);
    posDelta_A = Vector2(0, velocity_A.y);
    posDelta_B = Vector2(0, velocity_B.y);
    SET_TRANSFORMS(1);
    searchY = !colliding(a,b);
    if(!(searchX || searchY)) {
        searchX = true;
        searchY = true;
    }
    //TODO: rotation is another axis
    // do binary search on each axis
    double binarySearchResult = 0;
    if(searchY) {
        posDelta_A = Vector2(0, velocity_A.y);
        posDelta_B = Vector2(0, velocity_B.y);
        BINARY_SEARCH(binarySearchResult);
        SET_TRANSFORMS(binarySearchResult);
        posInitial_A.y += posDelta_A.y * binarySearchResult;
        posInitial_B.y += posDelta_B.y * binarySearchResult;
    }
    if(searchX) {
        posDelta_A = Vector2(velocity_A.x, 0);
        posDelta_B = Vector2(velocity_B.x, 0);
        BINARY_SEARCH(binarySearchResult);
        SET_TRANSFORMS(binarySearchResult);
        posInitial_A.x += posDelta_A.x * binarySearchResult;
        posInitial_B.x += posDelta_B.x * binarySearchResult;
    }
    //TODO: handle velocity
}

void CollisionSystem::resolveCollisions(vector<Collision> &collisions) {
    for (Collision collision : collisions) {
        if(!(collision.a.collider->isTrigger || collision.b.collider->isTrigger)) {
            resolveCollision(collision.a, collision.b);
        }
    }
}
#pragma endregion