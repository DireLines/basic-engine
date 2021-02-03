#ifndef INPUT_H
#define INPUT_H

#include <set>
#include <tuple>
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

    static tuple<SDL_Point, SDL_Point> selectionCorners() {
        // TODO: figure out top left / bottom right points, return those
        return make_tuple(lastClickedPosition, cursorPosition);
    }
    static SDL_Point getCursorPosition() {
        return cursorPosition;
    }
    static SDL_Point getLastClickedPosition() {
        return lastClickedPosition;
    }
    static bool getLeftButtonHeld() {
        return leftButtonHeld;
    }
    static bool getRightButtonHeld() {
        return rightButtonHeld;
    }
    static bool getLeftButtonDown() {
        return leftButtonDown;
    }
    static bool getRightButtonDown() {
        return rightButtonDown;
    }
private:
    static void poll(SDL_Event event);
    static void update();
    static set<SDL_Scancode> keys;
    static set<char> buttons;
    static set<char> clicks;
    static SDL_Point cursorPosition;
    static SDL_Point lastClickedPosition;
    static bool leftButtonHeld, rightButtonHeld, leftButtonDown, rightButtonDown;
    friend class Game;
};
#endif