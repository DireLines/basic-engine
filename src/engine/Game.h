#ifndef GAME_H
#define GAME_H

#include <vector>
#include <set>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "IDGenerator.h"
#include "GameObjects.h"
#include "ScriptRunner.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "GameTimer.h"
#include "Input.h"

using namespace std;

class Game {

public:

    /* Singleton pattern */
    static Game* instance;

    int frames_per_sec = 120;
    int windowWidth, windowHeight;

    SDL_Window * window;
    static SDL_Renderer* renderer;

    //Global frame counter
    static unsigned int frameCounter;

    Game(int windowWidth, int windowHeight);
    virtual ~Game();
    void start();
    void initialize(); //game-specific initialization of objects

    virtual bool update();

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

    set<GameObject*> objects;

    vector<System*> systems;

    set<GameObject*> objectsToDelete;

    double averageFrameLength = 0;
};

#endif