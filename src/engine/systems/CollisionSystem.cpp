#include "CollisionSystem.h"
#include "Sprite.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}

void CollisionSystem::update() {
    update_endpoint_positions();
    sort_intervals();

    /*debug*/
    SDL_Color color = {255, 255, 255};
    for (int i = 0; i < intervals.size(); ++i) {
        intervals[i]->object->transform->gameObject->getComponent<Sprite>()->color = color;
    }
    color = {0, 200, 0};
    /*debug*/

    for (int i = 0; i < intervals.size(); ++i) {
        ColliderTransform* o1 = intervals[i]->object;
        int j = i + 1;
        while ((j < intervals.size()) && (intervals[j]->begin <= intervals[i]->end)) {
            ColliderTransform* o2 = intervals[j]->object;
            if (!(o1->collider->enabled && o2->collider->enabled)) {
                j++; continue;
            }
            //you should not be able to collide with yourself
            if (o1->transform->gameObject == o2->transform->gameObject) {
                j++; continue;
            }
            if (GJK_collide(o1, o2)) {
                //TODO: call collision events on scripts, resolve collisions
                /*debug*/
                o1->transform->gameObject->getComponent<Sprite>()->color = color;
                o2->transform->gameObject->getComponent<Sprite>()->color = color;
                /*debug*/
            }
            j++;
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

bool CollisionSystem::GJK_collide(ColliderTransform* a, ColliderTransform* b) {
    MinkowskiDifferenceSupport s(a, b);
    Vector2 origin(0, 0);
    Vector2 p1 = s(Vector2(1, 0));
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

bool CollisionSystem::colliding(GameObject* a, GameObject* b) {
    return false;
}

void CollisionSystem::resolveCollision(GameObject* a, GameObject* b) {
}

bool beginning_pos(Interval* a, Interval* b) {
    return a->begin < b->begin;
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

void CollisionSystem::sort_intervals() {
    std::sort(intervals.begin(), intervals.end(), beginning_pos);
}