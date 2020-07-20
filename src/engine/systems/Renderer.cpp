#include "Renderer.h"

Renderer::Renderer() {}
void Renderer::update() {
    //TODO: maintain property that objects is sorted by z
    for (GameObject* obj : objects) {
        draw(obj);
    }
}
void Renderer::addObject(GameObject* obj) {
    //TODO: maintain property that objects is sorted by z
    objects.insert(obj);
}
void Renderer::removeObject(GameObject* obj) {
    objects.erase(obj);
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