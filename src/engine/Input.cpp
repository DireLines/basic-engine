#include "Input.h"
Input* Input::instance;
set<SDL_Scancode> Input::pressedKeys;
Input::Input() {
    Input::instance = this;
}