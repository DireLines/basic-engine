#include "CollisionSystem.h"
#include "Sprite.h"

CollisionSystem::CollisionSystem() {
    name = "CollisionSystem";
}

//TODO: complete broad phase to reduce to O(n log n)
void CollisionSystem::update() {
    update_endpoint_positions();
    sort_endpoints();

    /*debug*/
    SDL_Color color = {255, 255, 255};
    for (int i = 0; i < endpoints.size(); ++i) {
        endpoints[i]->object->transform->gameObject->getComponent<Sprite>()->color = color;
    }
    color = {0, 200, 0};
    /*debug*/

    for (int i = 0; i < endpoints.size(); ++i) {
        if (!endpoints[i]->begin) {
            continue;
        }
        ColliderTransform* o1 = endpoints[i]->object;
        int j = i + 1;
        while ((j < endpoints.size()) && (endpoints[j]->object != o1)) {
            ColliderTransform* o2 = endpoints[j]->object;
            // cout << o1 << " " << o2 << endl;
            if (endpoints[j]->begin) {
                if (!(o1->collider->enabled && o2->collider->enabled)) {
                    j++; continue;
                }
                if (o1->transform->gameObject == o2->transform->gameObject) {
                    j++; continue;
                }
                if (GJK_collide(o1, o2)) {
                    /*debug*/
                    o1->transform->gameObject->getComponent<Sprite>()->color = color;
                    o2->transform->gameObject->getComponent<Sprite>()->color = color;
                    /*debug*/
                }
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
        IntervalEndpoint* begin = new IntervalEndpoint();
        begin->begin = true;
        begin->pos = 0;//will be overwritten
        begin->object = ct;
        endpoints.push_back(begin);
        IntervalEndpoint* end = new IntervalEndpoint();
        end->begin = false;
        end->pos = 0;//will be overwritten
        end->object = ct;
        endpoints.push_back(end);
    }
}
void CollisionSystem::removeObject(GameObject* obj) {
    //so that I reach the "end" endpoint after the "begin"
    //TODO: try to avoid this
    update_endpoint_positions();
    sort_endpoints();
    vector<IntervalEndpoint*>::iterator it = endpoints.begin();
    while (it != endpoints.end()) {
        if ((*it)->object->transform->gameObject == obj) {
            if (!(*it)->begin)
                delete (*it)->object;
            delete *it;
            it = endpoints.erase(it);
        } else {
            it++;
        }
    }
}

// bool CollisionSystem::y_bounds_overlap(ColliderTransform* a, ColliderTransform* b) {
//     Vector2 up(0, 1);
//     Vector2 down(0, -1);
//     Matrix3 a_m = a->transform->Apply();
//     Matrix3 b_m = b->transform->Apply();
//     double a_up = MinkowskiDifferenceSupport::transformedSupport(up,
//                   a_m,
//                   a->collider).y;
//     double a_down = MinkowskiDifferenceSupport::transformedSupport(down,
//                     a_m,
//                     a->collider).y;
//     double b_up = MinkowskiDifferenceSupport::transformedSupport(up,
//                   b_m,
//                   b->collider).y;
//     double b_down = MinkowskiDifferenceSupport::transformedSupport(down,
//                     b_m,
//                     b->collider).y;

// }

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

bool pos(IntervalEndpoint* a, IntervalEndpoint* b) {
    return a->pos < b->pos;
}

void CollisionSystem::update_endpoint_positions() {
    for (IntervalEndpoint* endpoint : endpoints) {
        Vector2 dir(1, 0);
        if (endpoint->begin) {
            dir = Vector2(-1, 0);
        }
        Matrix3 m = endpoint->object->transform->Apply();
        endpoint->pos = MinkowskiDifferenceSupport::transformedSupport(dir,
                        m,
                        endpoint->object->collider).x;
    }
}

void CollisionSystem::sort_endpoints() {
    std::sort(endpoints.begin(), endpoints.end(), pos);
}