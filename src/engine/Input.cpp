#include "Input.h"
Input* Input::instance;
set<SDL_Scancode> Input::pressedKeys;
set<char> Input::pressedButtons;
set<char> Input::clicks;
Input::Input() {
    Input::instance = this;
}