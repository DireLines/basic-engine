#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "DisplayObject.h"
#include <vector>
#include <set>
#include "IDGenerator.h"
#include "GameObjects.h"
#include "Vector2.h"
#include "ScriptRunner.h"

using namespace std;

class Game {

public:

    /* Singleton pattern */
    static Game* instance;

    int frames_per_sec = 60;
    int windowWidth, windowHeight;

    SDL_Window * window;
    static SDL_Renderer* renderer;

    //Global frame counter
    static unsigned int frameCounter;

    Game(int windowWidth, int windowHeight);
    virtual ~Game();
    void start();

    virtual void update();
    virtual void draw();

    void instantiate(GameObject* obj);
    // TODO: instantiate overloads for position & rotation and all that
    // void instantiate(GameObject* obj, Vector2 pos, double rotation);
    // void instantiate(GameObject* obj, Vector2 pos, double rotation, Vector2 scale);
    // void instantiate(GameObject* obj, Transform transform);

    void destroy(GameObject* obj);
    //TODO: destroy overload for object by id
    // void destroy(int id);
    // GameObject* objectByID(int id);

private:

    void initSDL();
    void quitSDL();
    set<SDL_Scancode> pressedKeys;

    set<GameObject*> objects;

    ScriptRunner* scriptRunner;

    double averageFrameLength = 0;
};

#endif