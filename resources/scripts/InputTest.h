#ifndef INPUTTEST_H
#define INPUTTEST_H

#include "Script.h"
class InputTest: public Script {
public:
    void update() {
        if (Input::keyPressed(SDL_SCANCODE_E)) {
            cout << "e pressed" << endl;
        }
        if (Input::keyHeld(SDL_SCANCODE_E)) {
            cout << "e held" << endl;
        }
        if (Input::keyReleased(SDL_SCANCODE_E)) {
            cout << "e released" << endl;
        }
        if (Input::getLeftButtonHeld()) {
            Vector2(Input::getCursorPosition()).print();
        }
    }
};

#endif