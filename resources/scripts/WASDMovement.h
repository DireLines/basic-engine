#ifndef WASDMOVEMENT_H
#define WASDMOVEMENT_H

#include "Script.h"
class WASDMovement: public Script {
public:
    double speed = 35000;
    Rigidbody* rb;
    void start() {
        rb = gameObject->getComponent<Rigidbody>();
    }
    void update() {
        Vector2 vel(0, 0);
        if (Input::getKey(SDL_SCANCODE_W)) {
            vel.y -= speed * GameTimer::DeltaTime();
        }
        if (Input::getKey(SDL_SCANCODE_S)) {
            vel.y += speed * GameTimer::DeltaTime();
        }
        if (Input::getKey(SDL_SCANCODE_A)) {
            vel.x -= speed * GameTimer::DeltaTime();
        }
        if (Input::getKey(SDL_SCANCODE_D)) {
            vel.x += speed * GameTimer::DeltaTime();
        }
        rb->velocity = vel;
    }
    bool parallelizable() {
        return true;
    }
};

#endif