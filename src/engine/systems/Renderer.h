#ifndef RENDERER_H
#define RENDERER_H

#include "GameObject.h"
#include "Script.h"
#include "System.h"
#include "Sprite.h"
#include "Transform.h"
#include <set>

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
    set<GameObject*> objects;
};

#endif