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
    string sprites_basepath = "./resources/sprites/";
    void addObject(GameObject* obj);
    SDL_Texture* addTexture(Sprite* sprite);
    void draw(GameObject* obj, Matrix3& cam_t);
    void sort_objects_by_z();
    double distance(Vector2 a, Vector2 b);
    double calculateRotation(Vector2 origin, Vector2 p);

    vector<GameObject*> objects;
    unordered_map<string, SDL_Texture*> textures;
    unordered_map<string, SDL_Surface*> images;
    Camera* camera = NULL;
};

#endif