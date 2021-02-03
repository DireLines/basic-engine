#ifndef INPUT_H
#define INPUT_H

#include <set>
#include <tuple>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

class Input {
public:
    /* Singleton pattern */
    static Input* instance;

    Input();
    ~Input();

    static bool keyPressed(SDL_Scancode key) {
        return keyHeld(key) && !keyHeldLastFrame(key);
    }
    static bool keyHeld(SDL_Scancode key) {
        return keys.find(key) != keys.end();
    }
    static bool keyReleased(SDL_Scancode key) {
        return !keyHeld(key) && keyHeldLastFrame(key);
    }

    static bool buttonPressed(char button) {
        return buttonHeld(button) && !buttonHeldLastFrame(button);
    }
    static bool buttonHeld(char button) {
        return buttons.find(button) != buttons.end();
    }
    static bool buttonReleased(char button) {
        return !buttonHeld(button) && buttonHeldLastFrame(button);
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
    static bool keyHeldLastFrame(SDL_Scancode key) {
        return prevKeys.find(key) != prevKeys.end();
    }
    static bool buttonHeldLastFrame(char button) {
        return prevButtons.find(button) != prevButtons.end();
    }
    static void poll(SDL_Event event);
    static void update();
    static void postUpdate();
    static set<SDL_Scancode> keys;
    static set<char> buttons;
    static set<char> clicks;
    static set<SDL_Scancode> prevKeys;
    static set<char> prevButtons;
    static set<char> prevClicks;
    static SDL_Point cursorPosition;
    static SDL_Point lastClickedPosition;
    static bool leftButtonHeld, rightButtonHeld, leftButtonDown, rightButtonDown;
    friend class Game;
};
#endif