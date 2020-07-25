#include "GameTimer.h"
GameTimer* GameTimer::instance;
double GameTimer::time = 0;
double GameTimer::deltaTime = 0;
GameTimer::GameTimer() {
    GameTimer::instance = this;
    time = 0;
    deltaTime = 0;
}