#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "DisplayObject.h"
#include <vector>
#include <set>
#include "GameObject.h"
#include "IDGenerator.h"
#include "Components.h"

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

private:

    void initSDL();
    void quitSDL();
    set<SDL_Scancode> pressedKeys;

};

#endif