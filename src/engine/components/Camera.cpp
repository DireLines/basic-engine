#include "Camera.h"
#include "Game.h"
#include "Spin.h"

Camera::Camera() {
    Transform* t = this->addComponent<Transform>();
    t->scale = Vector2(2, 2);
    Spin* s = this->addComponent<Spin>();
    s->speed = 1;
}