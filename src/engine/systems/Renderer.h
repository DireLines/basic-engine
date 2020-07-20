#ifndef RENDERER_H
#define RENDERER_H

#include "GameObject.h"
#include "System.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>

using namespace std;

class Renderer : public System {
public:
    Renderer();
    ~Renderer();
    void update();
    bool needObject(GameObject* obj);
    void addObject(GameObject* obj);
    void removeObject(GameObject* obj);
private:
    void draw(GameObject* obj);
    void sort_objects_by_z();
    vector<GameObject*> objects;
};

#endif