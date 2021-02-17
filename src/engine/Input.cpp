#include "Input.h"
Input* Input::instance;
set<SDL_Scancode> Input::keys;
set<char> Input::buttons;
set<char> Input::clicks;
set<SDL_Scancode> Input::prevKeys;
set<char> Input::prevButtons;
set<char> Input::prevClicks;
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

Input::~Input() {
    // SDL_JoystickClose( controller );
    // controller = NULL;
}


void Input::poll(SDL_Event event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        keys.insert(event.key.keysym.scancode);
        break;
    case SDL_KEYUP:
        keys.erase(event.key.keysym.scancode);
        break;
    case SDL_MOUSEBUTTONDOWN:
        clicks.insert(event.button.button);
        break;
    case SDL_MOUSEBUTTONUP:
        clicks.erase(event.button.button);
        break;
    case SDL_JOYBUTTONDOWN:
        buttons.insert(event.jbutton.button);
        break;
    case SDL_JOYBUTTONUP:
        buttons.erase(event.jbutton.button);
        break;
    }
}

void Input::update() {
    // leftStick = Vector2(0, 0);
    // Vector2 leftStickRaw = Vector2(0, 0);
    // short raw_left_x = SDL_JoystickGetAxis(controller, 0);
    // short raw_left_y = SDL_JoystickGetAxis(controller, 1);
    // leftStickRaw.x = (float)(raw_left_x) / SHRT_MAX;
    // leftStickRaw.y = (float)(raw_left_y) / SHRT_MAX;
    // if (leftStickRaw.magnitude() > deadZone) {
    //     leftStick = Vector2(leftStickRaw.x, leftStickRaw.y);
    // }

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
void Input::postUpdate() {
    prevKeys = set<SDL_Scancode>(keys);
    prevButtons = set<char>(buttons);
    prevClicks = set<char>(clicks);
}