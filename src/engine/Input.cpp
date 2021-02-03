#include "Input.h"
Input* Input::instance;
set<SDL_Scancode> Input::keys;
set<char> Input::buttons;
set<char> Input::clicks;
SDL_Point Input::cursorPosition = {0, 0};
SDL_Point Input::lastClickedPosition = {0, 0};
bool Input::leftButtonHeld, Input::rightButtonHeld, Input::leftButtonDown, Input::rightButtonDown = false;

Input::Input() {
    Input::instance = this;
    // controller = NULL;
    // SDL_Init(SDL_INIT_JOYSTICK);
    // if ( SDL_NumJoysticks() > 0 ) {
    //     cout << SDL_NumJoysticks() << " joysticks connected" << endl;
    //     controller = SDL_JoystickOpen(0);//first joystick
    // }
    // leftStick = Vector2(0, 0);
}

void Input::poll(SDL_Event event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        Input::keys.insert(event.key.keysym.scancode);
        break;
    case SDL_KEYUP:
        Input::keys.erase(event.key.keysym.scancode);
        break;
    case SDL_MOUSEBUTTONDOWN:
        Input::clicks.insert(event.button.button);
        break;
    case SDL_MOUSEBUTTONUP:
        Input::clicks.erase(event.button.button);
        break;
    case SDL_JOYBUTTONDOWN:
        Input::buttons.insert(event.jbutton.button);
        break;
    case SDL_JOYBUTTONUP:
        Input::buttons.erase(event.jbutton.button);
        break;
    }
}

void Input::update() {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    cursorPosition = {x, y};
    bool leftButtonThisFrame = false;
    leftButtonDown = false;
    bool rightButtonThisFrame = false;
    rightButtonDown = false;
    for (char click : clicks) {
        switch (click) {
        case SDL_BUTTON_LEFT:
            leftButtonThisFrame = true;
            if (!leftButtonHeld) {
                leftButtonHeld = true;
                leftButtonDown = true;
                lastClickedPosition = cursorPosition;
            }
            break;
        case SDL_BUTTON_RIGHT:
            rightButtonThisFrame = true;
            if (!rightButtonHeld) {
                rightButtonHeld = true;
                rightButtonDown = true;
            }
            break;
        default:
            break;
        }
    }
    if (!leftButtonThisFrame) {
        leftButtonHeld = false;
    }
    if (!rightButtonThisFrame) {
        rightButtonHeld = false;
    }
}