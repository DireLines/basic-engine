#include "Renderer.h"
#include "Game.h"
#include <algorithm>
#define VECTOR_ERASE(v,value) (v.erase(std::remove(v.begin(), v.end(), value), v.end()))
#define VECTOR_DEDUP(v) (v.erase(std::unique(v.begin(), v.end()), v.end()))

Renderer::Renderer() {}

bool z_less(GameObject* a, GameObject* b) {
    return a->getComponent<Sprite>()->z < b->getComponent<Sprite>()->z;
}

//objects is almost always sorted,
//and checking if sorted is faster than sorting
void Renderer::sort_objects_by_z() {
    if (!std::is_sorted(objects.begin(), objects.end(), z_less)) {
        std::sort(objects.begin(), objects.end(), z_less);
    }
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
    SDL_Texture* tex = addTexture(obj->getComponent<Sprite>()->filename);
    obj->getComponent<Sprite>()->setTexture(tex);
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
        Transform* cam_t = camera->getComponent<Transform>();
        Transform* obj_t = obj->getComponent<Transform>();
        //TODO: draw the sprite
    }
}

SDL_Texture* Renderer::addTexture(string filename) {
    SDL_Texture* result = SDL_CreateTextureFromSurface(Game::renderer,  IMG_Load(filename.c_str()));
    textures[filename] = result;
    return result;
}