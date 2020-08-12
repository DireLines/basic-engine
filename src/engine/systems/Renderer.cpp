#include "Renderer.h"
#include "Game.h"
#include <algorithm>
#define VECTOR_ERASE(v,value) ((v).erase(std::remove((v).begin(), (v).end(), (value)), (v).end()))
#define VECTOR_DEDUP(v) ((v).erase(std::unique((v).begin(), (v).end()), (v).end()))
#define UINT8(d) ((int)((d) * 255) % 256)

Renderer::Renderer() {
    name = "Renderer";
    //TODO: set camera based on information from scene
    camera = new Camera();
    Game::instance->instantiate(camera);
}

void Renderer::update() {
    SDL_RenderClear(Game::renderer);
    sort_objects_by_z();
    Matrix3 center = Transform::Translate(
                         Game::instance->windowWidth / 2,
                         Game::instance->windowHeight / 2
                     );
    Matrix3 cam_t = center * camera->getComponent<Transform>()->Reverse();
    for (SpriteTransform* obj : objects) {
        draw(obj, cam_t);
    }
    SDL_RenderPresent(Game::renderer);
}

bool z_less(SpriteTransform* a, SpriteTransform* b) {
    return a->sprite->z < b->sprite->z;
}

//objects is almost always sorted,
//and checking if sorted is faster than sorting
void Renderer::sort_objects_by_z() {
    if (!std::is_sorted(objects.begin(), objects.end(), z_less)) {
        std::stable_sort(objects.begin(), objects.end(), z_less);
    }
}

void Renderer::addObject(GameObject* obj) {
    Sprite* s = obj->getComponent<Sprite>();
    Transform* t = obj->getComponent<Transform>();

    SpriteTransform* st = new SpriteTransform();
    st->sprite = s;
    st->transform = t;
    objects.push_back(st);

    VECTOR_DEDUP(objects);
    addTexture(s);
    SDL_Surface* img = s->image;
    //TODO: figure out how to not set this
    t->pivot = Vector2(img->w / 2, img->h / 2);
}
void Renderer::removeObject(GameObject* obj) {
    for (SpriteTransform* st : objects) {
        if (st->sprite->gameObject == obj) {
            VECTOR_ERASE(objects, st);
            return;
        }
    }
}
bool Renderer::needObject(GameObject* obj) {
    return obj->hasComponent<Sprite>() && obj->hasComponent<Transform>();
}

void Renderer::draw(SpriteTransform* obj, Matrix3& cam_t) {
    Sprite* s = obj->sprite;
    if (s && s->enabled) {
        SDL_Texture* texture = s->texture;
        SDL_Surface* image = s->image;
        if (texture) {
            Transform* obj_t = obj->transform;
            Matrix3 transform = cam_t * obj_t->Apply() * obj_t->Unpivot();
            Vector2 topLeft = transform * Vector2(0, 0);
            Vector2 topRight = transform * Vector2(image->w, 0);
            Vector2 bottomRight = transform * Vector2(image->w, image->h);

            int w = (int)round(Vector2::distance(topLeft, topRight));
            int h = (int)round(Vector2::distance(topRight, bottomRight));

            double displayAngle = Vector2::calculateRotation(topLeft, topRight);
            SDL_Point origin = topLeft.toPixel();
            SDL_Point corner = {0, 0};

            SDL_Rect dstrect = { origin.x, origin.y, w, h};
            SDL_SetTextureAlphaMod(texture, UINT8(s->alpha));
            SDL_RenderCopyEx(Game::renderer, texture, NULL, &dstrect, displayAngle, &corner, SDL_FLIP_NONE);
        }
    }
}

SDL_Texture* Renderer::addTexture(Sprite* sprite) {
    string filename = sprites_basepath + sprite->file;
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