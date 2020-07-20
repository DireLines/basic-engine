#include "Renderer.h"
#include "Game.h"
#include <algorithm>
#define VECTOR_ERASE(v,value) ((v).erase(std::remove((v).begin(), (v).end(), (value)), (v).end()))
#define VECTOR_DEDUP(v) ((v).erase(std::unique((v).begin(), (v).end()), (v).end()))
#define UINT8(d) ((int)((d) * 255) % 256)

Renderer::Renderer() {
    camera = new Camera();
    Game::instance->instantiate(camera);
}

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
    SDL_RenderClear(Game::renderer);
    sort_objects_by_z();
    for (GameObject* obj : objects) {
        draw(obj);
    }
    SDL_RenderPresent(Game::renderer);
}
void Renderer::addObject(GameObject* obj) {
    objects.push_back(obj);
    VECTOR_DEDUP(objects);
    addTexture(obj->getComponent<Sprite>());
}
void Renderer::removeObject(GameObject* obj) {
    VECTOR_ERASE(objects, obj);
}
bool Renderer::needObject(GameObject* obj) {
    return obj->hasComponent<Sprite>() && obj->hasComponent<Transform>();
}

void Renderer::draw(GameObject* obj) {
    Sprite* s = obj->getComponent<Sprite>();
    if (s && s->enabled) {
        Transform* cam_t = camera->getComponent<Transform>();
        Transform* obj_t = obj->getComponent<Transform>();
        SDL_Texture* texture = s->texture;
        SDL_Surface* image = s->image;
        cout << image->w << " " << image->h << endl;
        //TODO: figure out where the object is on screen
        if (texture) {
            SDL_Rect dstrect = { 0, 0, image->w , image->h};
            SDL_SetTextureAlphaMod(texture, UINT8(s->alpha));
            SDL_RenderCopyEx(Game::renderer, texture, NULL, &dstrect, 0, NULL, SDL_FLIP_NONE);
        }
    }
}

SDL_Texture* Renderer::addTexture(Sprite* sprite) {
    string filename = sprites_basepath + sprite->filename;
    if (textures.find(filename) == textures.end()) {
        SDL_Surface* image = IMG_Load(filename.c_str());
        if (!image) {
            printf("IMG_Load: %s %s\n", IMG_GetError(), filename.c_str());
            image = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0x000000ff);
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer,  image);
        images[filename] = image;
        textures[filename] = texture;
    }
    sprite->image = images[filename];
    sprite->texture = textures[filename];
    return textures[filename];
}