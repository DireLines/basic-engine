#include "Input.h"
Input* Input::instance;
set<SDL_Scancode> Input::pressedKeys;
set<char> Input::pressedButtons;
set<char> Input::clicks;
SDL_Point Input::cursorPosition = {0, 0};
SDL_Point Input::lastClickedPosition = {0, 0};
bool Input::leftButtonHeld, Input::rightButtonHeld, Input::leftButtonDown, Input::rightButtonDown = false;

Input::Input() {
    Input::instance = this;
    // controller = NULL;
    // SDL_Init(SDL_INIT_JOYSTICK);
    // if ( SDL_NumJoysticks() < 1 ) {
    //     cout << "No joysticks connected" << endl;
    // } else {
    //     cout << SDL_NumJoysticks() << " joysticks connected" << endl;
    //     controller = SDL_JoystickOpen(0);//first joystick
    // }
    // leftStick = Vector2(0, 0);
}

// void Input::update(set<char> clicks) {
//     int x;
//     int y;
//     SDL_GetMouseState(&x, &y);
//     cursorPosition = {x, y};
//     bool leftButtonThisFrame = false;
//     leftButtonDown = false;
//     bool rightButtonThisFrame = false;
//     rightButtonDown = false;
//     for (char click : clicks) {
//         switch (click) {
//         case SDL_BUTTON_LEFT:
//             leftButtonThisFrame = true;
//             if (!leftButtonHeld) {
//                 leftButtonHeld = true;
//                 leftButtonDown = true;
//                 lastClickedPosition = cursorPosition;
//             }
//             break;
//         case SDL_BUTTON_RIGHT:
//             rightButtonThisFrame = true;
//             if (!rightButtonHeld) {
//                 rightButtonHeld = true;
//                 rightButtonDown = true;
//             }
//             break;
//         default:
//             break;
//         }
//     }
//     if (!leftButtonThisFrame) {
//         leftButtonHeld = false;
//     }
//     if (!rightButtonThisFrame) {
//         rightButtonHeld = false;
//     }
// }