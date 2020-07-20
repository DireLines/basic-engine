#ifndef RENDERER_H
#define RENDERER_H

#include "GameObject.h"
#include "System.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Camera.h"

using namespace std;

class Renderer : public System {
public:
    Renderer();
    ~Renderer();
    void update();
    bool needObject(GameObject* obj);
    void removeObject(GameObject* obj);
    Camera* getCamera() {return camera;}
    void setCamera(Camera* cam) {camera = cam;}
private:
    void addObject(GameObject* obj);
    SDL_Texture* addTexture(string filename);
    void draw(GameObject* obj);
    void sort_objects_by_z();
    vector<GameObject*> objects;
    unordered_map<string, SDL_Texture*> textures;
    Camera* camera = NULL;
};

#endif