#ifndef INPUT_H
#define INPUT_H

#include <set>
#include <SDL2/SDL.h>

using namespace std;

class Input {
public:
    /* Singleton pattern */
    static Input* instance;

    Input();
    ~Input();

    static bool getKey(SDL_Scancode key) {
        return pressedKeys.find(key) != pressedKeys.end();
    }
private:
    static set<SDL_Scancode> pressedKeys;
    friend class Game;
};
#endif