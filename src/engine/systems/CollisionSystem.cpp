#include "CollisionSystem.h"
#include "Sprite.h" //debug

const auto processor_count = std::thread::hardware_concurrency();

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}

void CollisionSystem::start() {
    //TODO: spawn some threads and put them in a pool
}

void CollisionSystem::update() {
    update_endpoint_positions();
    sort_intervals();
    precalculate_matrices();


    /*debug*/
    SDL_Color color = {255, 255, 255};
    for (Interval* interval : intervals) {
        interval->object->transform->gameObject->getComponent<Sprite>()->color = color;
    }
    /*debug*/

    vector<thread> threads;

    for (int i = 0; i < processor_count; ++i) {
        threads.push_back(thread(&CollisionSystem::detect_collisions, this, i));
    }
    for (int i = 0; i < processor_count; ++i) {
        threads[i].join();
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
        Interval* interval = new Interval();
        interval->begin = 0;//will be overwritten
        interval->end = 0;////will be overwritten
        interval->object = ct;
        intervals.push_back(interval);
    }
}
void CollisionSystem::removeObject(GameObject* obj) {
    vector<Interval*>::iterator it = intervals.begin();
    while (it != intervals.end()) {
        if ((*it)->object->transform->gameObject == obj) {
            delete (*it)->object;
            delete *it;
            it = intervals.erase(it);
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

bool CollisionSystem::GJK_collide(ColliderMatrices& a, ColliderMatrices& b) {
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
        //determine whether to keep p1's value, replace it with p2's, or recreate both p1 and p2
        bool edge_p3p1 = inEdgeRegionAB(origin, p3, p1, p2);
        bool edge_p3p2 = inEdgeRegionAB(origin, p3, p2, p1);
        //swap points to converge triangle on origin
        //by the end, p1 and p2 should be the vertices of the closest edge to the origin
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

// bool CollisionSystem::GJK_collide(ColliderTransform* a, ColliderTransform* b) {
// return GJK_collide(a->transform->Apply(), a->collider, b->transform->Apply(), b->collider);
// }

bool CollisionSystem::colliding(GameObject* a, GameObject* b) {
    return false;
}

void CollisionSystem::resolveCollision(GameObject* a, GameObject* b) {
}

void CollisionSystem::update_endpoint_positions() {
    for (Interval* interval : intervals) {
        Matrix3 m = interval->object->transform->Apply();
        interval->begin = MinkowskiDifferenceSupport::transformedSupport(Vector2(-1, 0),
                          m, interval->object->collider).x;
        interval->end = MinkowskiDifferenceSupport::transformedSupport(Vector2(1, 0),
                        m, interval->object->collider).x;
    }
}

bool beginning_pos(Interval* a, Interval* b) {
    return a->begin < b->begin;
}


void CollisionSystem::sort_intervals() {
    std::sort(intervals.begin(), intervals.end(), beginning_pos);
}

void CollisionSystem::precalculate_matrices() {
    for (Interval* interval : intervals) {
        interval->precalculated.applied_transform = interval->object->transform->Apply();
        interval->precalculated.undo_rotation = Transform::Rotate(-interval->precalculated.applied_transform.rotation());
    }
}
void CollisionSystem::detect_collisions(int thread_id) {
    SDL_Color color = {0, 200, 0};
    int start = MathUtils::getThreadStartIndex(0, intervals.size(), thread_id, processor_count);
    int stop = MathUtils::getThreadStartIndex(0, intervals.size(), thread_id + 1, processor_count);
    for (int i = start; i < stop && i < intervals.size(); ++i) {
        ColliderMatrices o1 = intervals[i]->precalculated;
        int j = i + 1;
        while ((j < intervals.size()) && (intervals[j]->begin <= intervals[i]->end)) {
            ColliderMatrices o2 = intervals[j]->precalculated;
            if (!(o1.collider->enabled && o2.collider->enabled)) {
                j++; continue;
            }
            //you should not be able to collide with yourself
            if (o1.collider->gameObject == o2.collider->gameObject) {
                j++; continue;
            }
            if (GJK_collide(o1, o2)) {
                //TODO: call collision events on scripts, resolve collisions
                /*debug*/
                o1.collider->gameObject->getComponent<Sprite>()->color = color;
                o2.collider->gameObject->getComponent<Sprite>()->color = color;
                /*debug*/
            }
            j++;
        }
    }
}