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
        double dt = GameTimer::DeltaTime();
        if (Input::keyHeld(SDL_SCANCODE_W)) {
            vel.y -= speed * dt;
        }
        if (Input::keyHeld(SDL_SCANCODE_S)) {
            vel.y += speed * dt;
        }
        if (Input::keyHeld(SDL_SCANCODE_A)) {
            vel.x -= speed * dt;
        }
        if (Input::keyHeld(SDL_SCANCODE_D)) {
            vel.x += speed * dt;
        }
        if (vel.magnitude() > speed * dt) {
            vel = vel.normalized() * speed * dt;
        }
        rb->velocity = vel;
    }
};

#endif