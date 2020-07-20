#include "Renderer.h"
#include <algorithm>
#define VECTOR_ERASE(v,value) (v.erase(std::remove(v.begin(), v.end(), value), v.end()))
#define VECTOR_DEDUP(v) (v.erase(std::unique(v.begin(), v.end()), v.end()))

Renderer::Renderer() {}

bool z_greater(GameObject* a, GameObject* b) {
    return a->getComponent<Sprite>()->z < b->getComponent<Sprite>()->z;
}

void Renderer::sort_objects_by_z() {
    std::sort(objects.begin(), objects.end(), z_greater);
}

void Renderer::update() {
    sort_objects_by_z();
    for (GameObject* obj : objects) {
        draw(obj);
    }
}
void Renderer::addObject(GameObject* obj) {
    objects.push_back(obj);
    VECTOR_DEDUP(objects);
    sort_objects_by_z();
}
void Renderer::removeObject(GameObject* obj) {
    VECTOR_ERASE(objects, obj);
}
bool Renderer::needObject(GameObject* obj) {
    return obj->hasComponent<Sprite>() && obj->hasComponent<Transform>();
}

void Renderer::draw(GameObject* obj) {
    Sprite* s = obj->getComponent<Sprite>();
    if (s->enabled) {
        Transform* t = obj->getComponent<Transform>();
        //TODO: draw the sprite
    }
}