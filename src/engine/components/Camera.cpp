#include "Camera.h"
#include "Game.h"

Camera::Camera() {
    Transform* t = this->addComponent<Transform>();
}